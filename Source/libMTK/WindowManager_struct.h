#pragma once

#include <MDK/Result.h>
#include <MTK/WindowManager.h>

struct MTK_WindowManager_struct {
  MDK_Result (*init)(MTK_WindowManager* windowManager);
  
  void* data;
};
