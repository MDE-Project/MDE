#pragma once

#include <MDK/Event.h>
#include <MDK/EventLoopImpl.h>

typedef struct {
  MDK_Event inherited;
  
  int argc;
  char** argv;
} MDK_Application_StartEvent;

MDK_Event_createTargetType(MDK_Application_StartEvent);

int MDK_Application_startWithEventLoopImpl(int argc, char** argv, MDK_EventLoopImpl* eventLoopImpl);
int MDK_Application_start(int argc, char** argv);

void MDK_Application_sendEvent(MDK_Event* event);
void MDK_Application_quit();

void MDK_Application_onStart(MDK_Application_StartEvent_Target target, void* data);
void MDK_Application_onQuitRequest(MDK_Event_Target target, void* data);
