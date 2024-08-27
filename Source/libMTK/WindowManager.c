#include <stdio.h>
#include <stdlib.h>

#include <MDK/Object.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>
#include <MTK/WindowManager/Wayland.h>

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

MTK_WindowManager* MTK_WindowManager_createOptimal() {
  MTK_WindowManager* windowManager;
  
  if (MTK_WindowManager_Wayland_create((MTK_WindowManager_Wayland**)&windowManager) == MDK_Result_success) {
    return windowManager;
  } else {
    fputs("MTK_WindowManager: No functional window manager implementation. Aborting!\n", stderr);
    abort();
  }
}
