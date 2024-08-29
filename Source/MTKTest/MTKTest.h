#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>
#include <MTK/WindowManager.h>

MDK_TypeID_create(MTKTest, 0x2E1EEDC9);

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  MTK_WindowManager* windowManager;
} MTKTest;

MTKTest* MTKTest_create();
void MTKTest_init(MTKTest* this);
void MTKTest_destroy(MTKTest* this);
