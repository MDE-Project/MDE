#include <stdlib.h>
#include <xcb/xcb.h>

#include <MTK/WindowManagerBackend/X11.h>

MDK_Result MTK_WindowManagerBackend_X11_create(MTK_WindowManager** windowManager) {
  *windowManager = malloc(sizeof(MTK_WindowManager));
  
  xcb_connection_t* x11Connection = xcb_connect(NULL, NULL);
  (*windowManager)->data = x11Connection;
  
  if (xcb_connection_has_error(x11Connection) > 0) {
    xcb_disconnect(x11Connection);
    free(*windowManager);
    return MDK_Result_genericFailure;
  }
  
  return MDK_Result_success;
}
