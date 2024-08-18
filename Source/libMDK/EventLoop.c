#include <MDK/EventLoop.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_EventLoop* MDK_EventLoop_create(MDK_EventLoop_RunFunc run, MDK_EventLoop_SendEventFunc sendEvent) {
  MDK_EventLoop* this = OBJ_CREATE(MDK_EventLoop);
  MDK_EventLoop_init(this, run, sendEvent);
  return this;
}

void MDK_EventLoop_init(MDK_EventLoop* this, MDK_EventLoop_RunFunc run, MDK_EventLoop_SendEventFunc sendEvent) {
  this->id = MDK_EventLoop_typeID;
  this->run = run;
  this->sendEvent = sendEvent;
  this->quitRequestCallback = NULL;
}

void MDK_EventLoop_destroy(MDK_EventLoop* this) {
  return;
}

void MDK_EventLoop_run(MDK_EventLoop* this) {
  MDK_TypeID_ensure(this->id, MDK_EventLoop_typeID);
  this->run(this);
}

void MDK_EventLoop_sendEvent(MDK_EventLoop* this, MDK_Event* event) {
  MDK_TypeID_ensure(this->id, MDK_EventLoop_typeID);
  this->sendEvent(this, event);
}

void MDK_EventLoop_setQuitRequestCallback(MDK_EventLoop* this, MDK_EventLoop_QuitRequestCallback quitRequestCallback) {
  MDK_TypeID_ensure(this->id, MDK_EventLoop_typeID);
  this->quitRequestCallback = quitRequestCallback;
}
