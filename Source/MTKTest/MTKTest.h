#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>
#include <MTK/WindowManager/Wayland.h>

MDK_TypeID_create(MTKTest, 'M', 'T', 'K', 'T');

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  MTK_WindowManager_Wayland* windowManager;
} MTKTest;

MTKTest* MTKTest_create();
void MTKTest_init(MTKTest* this);
void MTKTest_destroy(MTKTest* this);
