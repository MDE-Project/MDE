#pragma once

#include <MDK/Event.h>

typedef struct {
  void (*init)(void (*quitRequestCallback)());
  void (*run)();
  void (*sendEvent)(MDK_Event* event);
} MDK_EventLoopImpl;
