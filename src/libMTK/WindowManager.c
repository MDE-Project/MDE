#include <stdlib.h>

#include <MTK/WindowManager.h>
#include <MTK/WindowManagerBackend/X11.h>

MTK_WindowManager* MTK_WindowManager_create() {
  MTK_WindowManager* windowManager;
  
  if (MTK_WindowManagerBackend_X11_create(&windowManager) == MDK_Result_success) {
    return windowManager;
  } else {
    abort();
  }
}
