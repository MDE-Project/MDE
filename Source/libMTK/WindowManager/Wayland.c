#include <MDK/Object.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>
#include <MTK/WindowManager/Wayland.h>

MTK_WindowManager_Wayland* MTK_WindowManager_Wayland_create() {
  MTK_WindowManager_Wayland* this = OBJ_CREATE(MTK_WindowManager_Wayland);
  MTK_WindowManager_Wayland_init(this);
  return this;
}

void MTK_WindowManager_Wayland_init(MTK_WindowManager_Wayland* this) {
  MTK_WindowManager_init(&this->inherited);
  this->id = MTK_WindowManager_Wayland_typeID;
}

void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this) {
  MDK_TypeID_ensure(this->id, MTK_WindowManager_Wayland_typeID);
  MTK_WindowManager_destroy(&this->inherited);
}
