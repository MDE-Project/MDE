#include <stdlib.h>
#include <xcb/xcb.h>

#include "../WindowManager_struct.h"
#include "X11.h"

static MDK_Result backendInit(MTK_WindowManager* windowManager) {
  xcb_connection_t* x11Connection = xcb_connect(NULL, NULL);
  
  if (xcb_connection_has_error(x11Connection) > 0) {
    xcb_disconnect(x11Connection);
    return MDK_Result_genericFailure;
  }
  
  windowManager->data = x11Connection;
  return MDK_Result_success;
} 

MTK_WindowManager* WindowManagerBackend_X11_create() {
  MTK_WindowManager* windowManager = malloc(sizeof(MTK_WindowManager));
  
  *windowManager = (MTK_WindowManager){
    .init = backendInit,
  };
  
  return windowManager;
}
