#include <wayland-client-core.h>
#include <wayland-client-protocol.h>

#include <MDK/Object.h>
#include <MDK/Result.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>
#include <MTK/WindowManager/Wayland.h>

MDK_Result MTK_WindowManager_Wayland_create(MTK_WindowManager_Wayland** this) {
  *this = OBJ_CREATE(MTK_WindowManager_Wayland);
  return MTK_WindowManager_Wayland_init(*this);
}

MDK_Result MTK_WindowManager_Wayland_init(MTK_WindowManager_Wayland* this) {
  MTK_WindowManager_init(&this->inherited);
  this->id = MTK_WindowManager_Wayland_typeID;
  
  this->display = wl_display_connect(NULL);
  if (!this->display) {
    return MDK_Result_genericFailure;
  }
  
  return MDK_Result_success;
}

void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this) {
  ENSURE(MTK_WindowManager_Wayland);
  MTK_WindowManager_destroy(&this->inherited);
  
  if (this->display) {
    wl_display_disconnect(this->display);
  }
}
