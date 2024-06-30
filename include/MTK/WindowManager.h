#pragma once

typedef struct {
  void* data;
  
  void (*init)();
} MTK_WindowManager;

MTK_WindowManager* MTK_WindowManager_create();
