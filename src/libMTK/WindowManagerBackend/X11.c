#include <stdlib.h>
#include <xcb/xcb.h>

#include <MDK/BackgroundTask.h>
#include "../WindowManager_struct.h"
#include "X11.h"

typedef struct {
  xcb_connection_t* x11Connection;
  const xcb_setup_t* x11Setup;
  xcb_screen_t* x11Screen;
} BackendState;

static void backendEventHandler(void* backendState_raw) {
  BackendState* backendState = backendState_raw;
  
  while (true) {
    xcb_generic_event_t* event = xcb_wait_for_event(backendState->x11Connection);
  }
}

static MDK_Result backendInit(MTK_WindowManager* windowManager) {
  BackendState* backendState = windowManager->data;
  
  backendState->x11Connection = xcb_connect(NULL, NULL);
  
  if (xcb_connection_has_error(backendState->x11Connection) > 0) {
    xcb_disconnect(backendState->x11Connection);
    return MDK_Result_genericFailure;
  }
  
  backendState->x11Setup = xcb_get_setup(backendState->x11Connection);
  
  xcb_screen_iterator_t screenIterator = xcb_setup_roots_iterator(backendState->x11Setup);
  backendState->x11Screen = screenIterator.data; // First screen
  
  MDK_BackgroundTask_create(backendEventHandler, backendState);
  
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
