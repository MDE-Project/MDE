#include <stdio.h>
#include <string.h>
#include <wayland-client-core.h>
#include <wayland-client-protocol.h>
#include <wayland-util.h>

#include <MDK/Object.h>
#include <MDK/Result.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>
#include <MTK/WindowManager/Wayland.h>

#define beginBind() \
  if (false) {}

#define tryBind(NAME, VERSION) \
  else if (!strcmp(interface, NAME##_interface.name) && version >= VERSION) { \
    wl_registry_bind(registry, name, &NAME##_interface, VERSION); \
    this->availableInterfaces.NAME = true; \
  }

static void registryListener_global(void* this_raw, struct wl_registry* registry, uint32_t name, const char* interface, uint32_t version) {
  CAST_THIS(MTK_WindowManager_Wayland);
  
  beginBind()
  tryBind(wl_compositor, 1)
}

static const struct wl_registry_listener registryListener = {
  .global = registryListener_global,
  .global_remove = NULL,
};

MDK_Result MTK_WindowManager_Wayland_create(MTK_WindowManager_Wayland** this) {
  *this = OBJ_CREATE(MTK_WindowManager_Wayland);
  return MTK_WindowManager_Wayland_init(*this);
}

MDK_Result MTK_WindowManager_Wayland_init(MTK_WindowManager_Wayland* this) {
  MTK_WindowManager_init(&this->inherited);
  this->id = MTK_WindowManager_Wayland_typeID;
  this->availableInterfaces.wl_compositor = false;
  
  this->display = wl_display_connect(NULL);
  if (!this->display) {
    fputs("MTK_WindowManager_Wayland: Failed to connect to Wayland\n", stderr);
    return MDK_Result_genericFailure;
  }
  
  // Negotiate common protocols with server
  struct wl_registry* registry = wl_display_get_registry(this->display);
  wl_registry_add_listener(registry, &registryListener, this);
  wl_display_roundtrip(this->display);
  
  if (!this->availableInterfaces.wl_compositor) {
    fputs("MTK_WindowManager_Wayland: Required interface wl_compositor unavailable\n", stderr);
    
    wl_registry_destroy(registry);
    return MDK_Result_genericFailure;
  }
  
  wl_registry_destroy(registry);
  
  return MDK_Result_success;
}

void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this) {
  ENSURE(MTK_WindowManager_Wayland);
  MTK_WindowManager_destroy(&this->inherited);
  
  if (this->display) {
    wl_display_disconnect(this->display);
  }
}
