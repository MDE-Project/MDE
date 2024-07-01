#include <stdlib.h>
#include <xcb/xcb.h>

#include "../WindowManager_struct.h"
#include "X11.h"

typedef struct {
  xcb_connection_t* x11Connection;
} BackendState;

static MDK_Result backendInit(MTK_WindowManager* windowManager) {
  BackendState* backendState = windowManager->data;
  
  backendState->x11Connection = xcb_connect(NULL, NULL);
  
  if (xcb_connection_has_error(backendState->x11Connection) > 0) {
    xcb_disconnect(backendState->x11Connection);
    return MDK_Result_genericFailure;
  }
  
  return MDK_Result_success;
} 

MTK_WindowManager* WindowManagerBackend_X11_create() {
  MTK_WindowManager* windowManager = malloc(sizeof(MTK_WindowManager));
  
  *windowManager = (MTK_WindowManager){
    .init = backendInit,
    .data = malloc(sizeof(BackendState)),
  };
  
  return windowManager;
}
