#pragma once

#include <stdbool.h>

#include <MDK/Object.h>
#include <MDK/Result.h>
#include <MDK/TypeID.h>
#include <MTK/WindowManager.h>

MDK_TypeID_create(MTK_WindowManager_Wayland, 'W', 'M', 'W', 'a');

// Opaque Wayland types
struct wl_display;

typedef struct {
  MTK_WindowManager inherited;
  MDK_TypeID id;
  
  struct wl_display* display;
  
  struct {
    bool wl_compositor : 1;
  } availableInterfaces;
} MTK_WindowManager_Wayland;

MDK_Result MTK_WindowManager_Wayland_create(MTK_WindowManager_Wayland** this);
MDK_Result MTK_WindowManager_Wayland_init(MTK_WindowManager_Wayland* this);
void MTK_WindowManager_Wayland_destroy(MTK_WindowManager_Wayland* this);
