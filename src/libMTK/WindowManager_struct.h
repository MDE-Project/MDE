#pragma once

#include <MDK/Result.h>

struct MTK_WindowManager_struct {
  MDK_Result (*init)(MTK_WindowManager* windowManager);
  
  void* data;
};
