#pragma once

#include <MDK/Event.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_EventLoop_StopEvent, 'E', 'L', 'S', 'E');

typedef struct {
  MDK_Event inherited;
  MDK_TypeID id;
  
  int exitCode;
} MDK_EventLoop_StopEvent;

MDK_EventLoop_StopEvent* MDK_EventLoop_StopEvent_create(int exitCode);
void MDK_EventLoop_StopEvent_init(MDK_EventLoop_StopEvent* this, int exitCode);
void MDK_EventLoop_StopEvent_destroy(MDK_EventLoop_StopEvent* this);

int MDK_EventLoop_StopEvent_getExitCode(MDK_EventLoop_StopEvent* this);
