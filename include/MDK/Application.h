#pragma once

#include <MDK/EventLoopImpl.h>

typedef struct {
  MDK_Event inherited;
  
  int argc;
  char** argv;
} MDK_Application_StartEvent;

int MDK_Application_startWithEventLoopImpl(int argc, char** argv, MDK_EventLoopImpl* eventLoopImpl);
int MDK_Application_start(int argc, char** argv);

void MDK_Application_sendEvent(MDK_Event* event);

void MDK_Application_onStart(void (*target)(MDK_Application_StartEvent* event));
