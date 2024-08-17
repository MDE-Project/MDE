#pragma once

#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_EventLoopImpl, 'E', 'L', 'I', 'm');

typedef void (*MDK_EventLoopImpl_PrepareFunc)(void (*quitRequestCallback)());
typedef void (*MDK_EventLoopImpl_RunFunc)();
typedef void (*MDK_EventLoopImpl_SendEventFunc)(MDK_Event* event);

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  MDK_EventLoopImpl_PrepareFunc prepare;
  MDK_EventLoopImpl_RunFunc run;
  MDK_EventLoopImpl_SendEventFunc sendEvent;
} MDK_EventLoopImpl;

MDK_EventLoopImpl* MDK_EventLoopImpl_create(MDK_EventLoopImpl_PrepareFunc prepare, MDK_EventLoopImpl_RunFunc run, MDK_EventLoopImpl_SendEventFunc sendEvent);
void MDK_EventLoopImpl_init(MDK_EventLoopImpl* this, MDK_EventLoopImpl_PrepareFunc prepare, MDK_EventLoopImpl_RunFunc run, MDK_EventLoopImpl_SendEventFunc sendEvent);
void MDK_EventLoopImpl_destroy(MDK_EventLoopImpl* this);

void MDK_EventLoopImpl_prepare(MDK_EventLoopImpl* this, void (*quitRequestCallback)());
void MDK_EventLoopImpl_run(MDK_EventLoopImpl* this);
void MDK_EventLoopImpl_sendEvent(MDK_EventLoopImpl* this, MDK_Event* event);
