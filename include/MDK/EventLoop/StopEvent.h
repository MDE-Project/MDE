#pragma once

#include <MDK/Event.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_EventLoop_StopEvent, 'E', 'L', 'S', 'E');

typedef struct {
  MDK_Event inherited;
  MDK_TypeID id;
} MDK_EventLoop_StopEvent;

MDK_EventLoop_StopEvent* MDK_EventLoop_StopEvent_create();
void MDK_EventLoop_StopEvent_init(MDK_EventLoop_StopEvent* this);
void MDK_EventLoop_StopEvent_destroy(MDK_EventLoop_StopEvent* this);
