#include <stdio.h>
#include <stdlib.h>

#include "Export.h"
#include <MTK/WindowManager.h>
#include "WindowManager_struct.h"
#include "WindowManagerBackend/X11.h"

Export MTK_WindowManager* MTK_WindowManager_create() {
  MTK_WindowManager* windowManager;
  
  windowManager = WindowManagerBackend_X11_create();
  if (windowManager->init(windowManager) == MDK_Result_success) {
    return windowManager;
  }
  
  free(windowManager);
  
  fputs("All windowing system backends failed! Aborting!\n", stderr);
  abort();
}
