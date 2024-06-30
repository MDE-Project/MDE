#pragma once

typedef struct {
  void (*init)();
} MTK_WindowManager;

MTK_WindowManager* MTK_WindowManager_create();
