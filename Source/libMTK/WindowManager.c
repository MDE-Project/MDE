#include <MDK/Object.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>

MTK_WindowManager* MTK_WindowManager_create() {
  MTK_WindowManager* this = OBJ_CREATE(MTK_WindowManager);
  MTK_WindowManager_init(this);
  return this;
}

void MTK_WindowManager_init(MTK_WindowManager* this) {
  this->id = MTK_WindowManager_typeID;
}

void MTK_WindowManager_destroy(MTK_WindowManager* this) {
  return;
}
