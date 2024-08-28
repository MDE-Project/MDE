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
  tryBind(seat, wl_seat_interface, 1)
  tryBind(shm, wl_shm_interface, 1)
  tryBind(wmBase, xdg_wm_base_interface, 1)
}

static const struct wl_registry_listener registryListener = {
  .global = registryListener_global,
  .global_remove = NULL,
};

static void dispatchWaylandEvents(MDK_Object* this_raw, MDK_Event* event) {
  CAST_THIS(MTK_WindowManager_Wayland);
  wl_display_dispatch_pending(this->display);
}

static void waylandEventTaskMain(MDK_Object* this_raw) {
  CAST_THIS(MTK_WindowManager_Wayland);
  
  struct pollfd pollInfo = {
    .fd = this->displayFd,
    .events = POLLIN,
  };
  
  while (true) {
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    wl_display_prepare_read(this->display);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    if (poll(&pollInfo, 1, -1) < 0) {
      fputs("MTK_WindowManager_Wayland: Error while polling Wayland socket\n", stderr);
      abort();
    }
    
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    wl_display_read_events(this->display);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    
    MDK_Application_pause();
      MDK_Event* event = MDK_Event_create(NULL, OBJ(this), dispatchWaylandEvents);
      MDK_Application_sendEvent(event);
    MDK_Application_resume();
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
  this->shm = NULL;
  this->wmBase = NULL;
  
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
  
  this->displayFd = wl_display_get_fd(this->display);
  this->waylandEventTask = MDK_BackgroundTask_create(OBJ(this), waylandEventTaskMain);
  REF(this->waylandEventTask);
  
  return MDK_Result_success;
}

void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this) {
  ENSURE(MTK_WindowManager_Wayland);
  MTK_WindowManager_destroy(&this->inherited);
  
  UNREF(this->waylandEventTask);
  
  if (this->compositor) {
    wl_compositor_destroy(this->compositor);
  }
  
  if (this->seat) {
    wl_seat_destroy(this->seat);
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
  
  if (this->display) {
    wl_display_disconnect(this->display);
  }
}
