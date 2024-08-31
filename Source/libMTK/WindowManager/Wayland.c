#include <poll.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-util.h>
#include <Wayland/xdg_shell.h>

#include <MDK/Application.h>
#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/Result.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>
#include <MTK/WindowManager/Wayland.h>

#define beginBind() \
  if (false) {}

#define tryBind(NAME, INTERFACE, VERSION) \
  else if (!strcmp(interface, INTERFACE.name) && version >= VERSION) { \
    this->NAME = wl_registry_bind(registry, name, &INTERFACE, VERSION); \
  }

static void registryListener_global(void* this_raw, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
  CAST_THIS(MTK_WindowManager_Wayland);
  
  beginBind()
  tryBind(compositor, wl_compositor_interface, 1)
  tryBind(seat, wl_seat_interface, 3)
  tryBind(shm, wl_shm_interface, 1)
  tryBind(wmBase, xdg_wm_base_interface, 1)
}

static const struct wl_registry_listener registryListener = {
  .global = registryListener_global,
  .global_remove = NULL,
};

static void seatListener_capabilities(void* this_raw, struct wl_seat* seat, uint32_t capabilities) {
  CAST_THIS(MTK_WindowManager_Wayland);
  
  if (capabilities & WL_SEAT_CAPABILITY_POINTER) {
    if (!this->pointer) {
      this->pointer = wl_seat_get_pointer(seat);
    }
  } else {
    if (this->pointer) {
      wl_pointer_release(this->pointer);
      this->pointer = NULL;
    }
  }
  
  if (capabilities & WL_SEAT_CAPABILITY_KEYBOARD) {
    if (!this->keyboard) {
      this->keyboard = wl_seat_get_keyboard(seat);
    }
  } else {
    if (this->keyboard) {
      wl_keyboard_release(this->keyboard);
      this->keyboard = NULL;
    }
  }
}

static void seatListener_name(void* this_raw, struct wl_seat* seat, const char* name) {
  return;
}

static const struct wl_seat_listener seatListener =  {
  .capabilities = seatListener_capabilities,
  .name = seatListener_name,
};

static void wmBaseListener_ping(void* data, struct xdg_wm_base* wmBase, uint32_t serial) {
  xdg_wm_base_pong(wmBase, serial);
}

static const struct xdg_wm_base_listener wmBaseListener = {
  .ping = wmBaseListener_ping,
};

static void dispatchWaylandEvents(MDK_Object* this_raw, MDK_Event* event) {
  CAST_THIS(MTK_WindowManager_Wayland);
  
  if (wl_display_prepare_read(this->display) != 0) {
    fputs("MTK_WindowManager_Wayland: Failed to prepare Wayland display for reading\n", stderr);
    abort();
  }
  
  wl_display_read_events(this->display);
  
  wl_display_dispatch_pending(this->display);
  wl_display_flush(this->display);
  
  if (wl_display_get_error(this->display) != 0) {
    fputs("MTK_WindowManager_Wayland: Error after dispatching Wayland events\n", stderr);
    abort();
  }
  
  pthread_mutex_lock(&this->waylandEventTaskMutex);
  pthread_cond_signal(&this->waylandEventsDispatched);
  pthread_mutex_unlock(&this->waylandEventTaskMutex);
}

static void waylandEventTaskMain(MDK_Object* this_raw) {
  CAST_THIS(MTK_WindowManager_Wayland);
  
  struct pollfd pollInfo = {
    .fd = this->displayFd,
    .events = POLLIN,
  };
  
  pthread_mutex_lock(&this->waylandEventTaskMutex);
  
  while (true) {
    if (poll(&pollInfo, 1, -1) < 0) {
      fputs("MTK_WindowManager_Wayland: Error while polling Wayland socket\n", stderr);
      abort();
    }
    
    MDK_Application_pause();
      MDK_Event* event = MDK_Event_create(NULL, OBJ(this), dispatchWaylandEvents);
      MDK_Application_sendEvent(event);
    MDK_Application_resume();
    
    pthread_cond_wait(&this->waylandEventsDispatched, &this->waylandEventTaskMutex);
  }
}

MDK_Result MTK_WindowManager_Wayland_create(MTK_WindowManager_Wayland** this) {
  *this = OBJ_CREATE(MTK_WindowManager_Wayland);
  return MTK_WindowManager_Wayland_init(*this);
}

#define checkInterface(NAME, INTERFACE) \
  if (!this->NAME) { \
    fputs("MTK_WindowManager_Wayland: Required interface " #INTERFACE " unavailable\n", stderr); \
    return MDK_Result_genericFailure; \
  }

MDK_Result MTK_WindowManager_Wayland_init(MTK_WindowManager_Wayland* this) {
  MTK_WindowManager_init(&this->inherited);
  this->id = MTK_WindowManager_Wayland_typeID;
  this->compositor = NULL;
  this->seat = NULL;
  this->pointer = NULL;
  this->keyboard = NULL;
  this->shm = NULL;
  this->wmBase = NULL;
  pthread_mutex_init(&this->waylandEventTaskMutex, NULL);
  pthread_cond_init(&this->waylandEventsDispatched, NULL);
  
  this->display = wl_display_connect(NULL);
  if (!this->display) {
    fputs("MTK_WindowManager_Wayland: Failed to connect to Wayland\n", stderr);
    return MDK_Result_genericFailure;
  }
  
  // Negotiate common protocols with server
  this->registry = wl_display_get_registry(this->display);
  wl_registry_add_listener(this->registry, &registryListener, this);
  
  wl_display_roundtrip(this->display);
  
  checkInterface(compositor, wl_compositor)
  checkInterface(seat, wl_seat)
  checkInterface(shm, wl_shm)
  checkInterface(wmBase, xdg_wm_base)
  
  wl_seat_add_listener(this->seat, &seatListener, this);
  xdg_wm_base_add_listener(this->wmBase, &wmBaseListener, NULL);
  
  wl_display_roundtrip(this->display);
  
  // Start listening to Wayland events in the background
  this->displayFd = wl_display_get_fd(this->display);
  this->waylandEventTask = MDK_BackgroundTask_create(OBJ(this), waylandEventTaskMain);
  REF(this->waylandEventTask);
  
  return MDK_Result_success;
}

void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this) {
  ENSURE(MTK_WindowManager_Wayland);
  MTK_WindowManager_destroy(&this->inherited);
  
  UNREF(this->waylandEventTask);
  pthread_cond_destroy(&this->waylandEventsDispatched);
  pthread_mutex_destroy(&this->waylandEventTaskMutex);
  
  if (this->compositor) {
    wl_compositor_destroy(this->compositor);
  }
  
  if (this->seat) {
    wl_seat_destroy(this->seat);
  }
  
  if (this->pointer) {
    wl_pointer_release(this->pointer);
  }
  
  if (this->keyboard) {
    wl_keyboard_release(this->keyboard);
  }
  
  if (this->shm) {
    wl_shm_destroy(this->shm);
  }
  
  if (this->wmBase) {
    xdg_wm_base_destroy(this->wmBase);
  }
  
  if (this->registry) {
    wl_registry_destroy(this->registry);
  }
  
  wl_display_roundtrip(this->display);
  
  if (this->display) {
    wl_display_disconnect(this->display);
  }
}
