#include <stdio.h>

#include <MDK/Object.h>
#include <MDK/Shorthand.h>
#include <MTK/WindowManager.h>

#include "MTKTest.h"

MTKTest* MTKTest_create() {
  MTKTest* this = OBJ_CREATE(MTKTest);
  MTKTest_init(this);
  return this;
}

void MTKTest_init(MTKTest* this) {
  this->id = MTKTest_typeID;
  this->windowManager = MTK_WindowManager_createOptimal();
  REF(this->windowManager);
}

void MTKTest_destroy(MTKTest* this) {
  ENSURE(MTKTest);
  
  UNREF(this->windowManager);
  puts("Bye!");
}
