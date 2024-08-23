#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>
#include <MTK/WindowManager.h>

MDK_TypeID_create(MTK_WindowManager_Wayland, 'W', 'M', 'W', 'a');

typedef struct {
  MTK_WindowManager inherited;
  MDK_TypeID id;
} MTK_WindowManager_Wayland;

MTK_WindowManager_Wayland* MTK_WindowManager_Wayland_create();
void MTK_WindowManager_Wayland_init(MTK_WindowManager_Wayland* this);
void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this);
