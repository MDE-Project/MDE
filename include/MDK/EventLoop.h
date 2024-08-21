#pragma once

#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/TypeID.h>

typedef struct MDK_EventLoop_struct MDK_EventLoop;
MDK_TypeID_create(MDK_EventLoop, 'E', 'L', 'o', 'o');

typedef void (*MDK_EventLoop_RunFunc)(MDK_EventLoop* this);
typedef void (*MDK_EventLoop_PauseFunc)(MDK_EventLoop* this);
typedef void (*MDK_EventLoop_ResumeFunc)(MDK_EventLoop* this);
typedef void (*MDK_EventLoop_SendEventFunc)(MDK_EventLoop* this, MDK_Event* event);

struct MDK_EventLoop_struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  MDK_EventLoop_RunFunc run;
  MDK_EventLoop_PauseFunc pause;
  MDK_EventLoop_ResumeFunc resume;
  MDK_EventLoop_SendEventFunc sendEvent;
};

MDK_EventLoop* MDK_EventLoop_create(MDK_EventLoop_RunFunc run, MDK_EventLoop_PauseFunc pause, MDK_EventLoop_ResumeFunc resume, MDK_EventLoop_SendEventFunc sendEvent);
void MDK_EventLoop_init(MDK_EventLoop* this, MDK_EventLoop_RunFunc run, MDK_EventLoop_PauseFunc pause, MDK_EventLoop_ResumeFunc resume, MDK_EventLoop_SendEventFunc sendEvent);
void MDK_EventLoop_destroy(MDK_EventLoop* this);

void MDK_EventLoop_run(MDK_EventLoop* this);
void MDK_EventLoop_pause(MDK_EventLoop* this);
void MDK_EventLoop_resume(MDK_EventLoop* this);
void MDK_EventLoop_sendEvent(MDK_EventLoop* this, MDK_Event* event);
