#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>

typedef struct MTK_WindowManager_struct MTK_WindowManager;
MDK_TypeID_create(MTK_WindowManager, 0x456554BF);

struct MTK_WindowManager_struct {
  MDK_Object inherited;
  MDK_TypeID id;
};

MTK_WindowManager* MTK_WindowManager_create();
void MTK_WindowManager_init(MTK_WindowManager* this);
void MTK_WindowManager_destroy(MTK_WindowManager* this);

MTK_WindowManager* MTK_WindowManager_createOptimal();
