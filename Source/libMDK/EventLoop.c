#include <MDK/EventLoop.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_EventLoop* MDK_EventLoop_create(MDK_EventLoop_RunFunc run, MDK_EventLoop_PauseFunc pause, MDK_EventLoop_ResumeFunc resume, MDK_EventLoop_SendEventFunc sendEvent) {
  MDK_EventLoop* this = OBJ_CREATE(MDK_EventLoop);
  MDK_EventLoop_init(this, run, pause, resume, sendEvent);
  return this;
}

void MDK_EventLoop_init(MDK_EventLoop* this, MDK_EventLoop_RunFunc run, MDK_EventLoop_PauseFunc pause, MDK_EventLoop_ResumeFunc resume, MDK_EventLoop_SendEventFunc sendEvent) {
  this->id = MDK_EventLoop_typeID;
  this->run = run;
  this->pause = pause;
  this->resume = resume;
  this->sendEvent = sendEvent;
}

void MDK_EventLoop_destroy(MDK_EventLoop* this) {
  return;
}

int MDK_EventLoop_run(MDK_EventLoop* this) {
  ENSURE(MDK_EventLoop);
  return this->run(this);
}

void MDK_EventLoop_pause(MDK_EventLoop* this) {
  ENSURE(MDK_EventLoop);
  this->pause(this);
}

void MDK_EventLoop_resume(MDK_EventLoop* this) {
  ENSURE(MDK_EventLoop);
  this->resume(this);
}

void MDK_EventLoop_sendEvent(MDK_EventLoop* this, MDK_Event* event) {
  ENSURE(MDK_EventLoop);
  this->sendEvent(this, event);
}
