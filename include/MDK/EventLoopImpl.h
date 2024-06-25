#pragma once

#include <MDK/Event.h>

typedef struct {
  void (*init)();
  void (*run)();
  void (*sendEvent)(MDK_Event* event);
} MDK_EventLoopImpl;
