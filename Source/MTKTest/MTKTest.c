#include <stdio.h>

#include <MDK/Object.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager/Wayland.h>

#include "MTKTest.h"

MTKTest* MTKTest_create() {
  MTKTest* this = OBJ_CREATE(MTKTest);
  MTKTest_init(this);
  return this;
}

void MTKTest_init(MTKTest* this) {
  this->id = MTKTest_typeID;
  
  if (MTK_WindowManager_Wayland_create(&this->windowManager) != MDK_Result_success) {
    puts("Failed to connect to Wayland");
  } else {
    puts("Connected to Wayland");
  }
  
  REF(this->windowManager);
}

void MTKTest_destroy(MTKTest* this) {
  ENSURE(MTKTest);
  
  UNREF(this->windowManager);
  puts("Bye!");
}
