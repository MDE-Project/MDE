#include <MDK/EventLoopImpl.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_EventLoopImpl* MDK_EventLoopImpl_create(MDK_EventLoopImpl_PrepareFunc prepare, MDK_EventLoopImpl_RunFunc run, MDK_EventLoopImpl_SendEventFunc sendEvent) {
  MDK_EventLoopImpl* this = OBJ_CREATE(MDK_EventLoopImpl);
  MDK_EventLoopImpl_init(this, prepare, run, sendEvent);
  return this;
}

void MDK_EventLoopImpl_init(MDK_EventLoopImpl* this, MDK_EventLoopImpl_PrepareFunc prepare, MDK_EventLoopImpl_RunFunc run, MDK_EventLoopImpl_SendEventFunc sendEvent) {
  this->id = MDK_EventLoopImpl_typeID;
  this->prepare = prepare;
  this->run = run;
  this->sendEvent = sendEvent;
}

void MDK_EventLoopImpl_destroy(MDK_EventLoopImpl* this) {
  return;
}

void MDK_EventLoopImpl_prepare(MDK_EventLoopImpl* this, void (*quitRequestCallback)()) {
  MDK_TypeID_ensure(this->id, MDK_EventLoopImpl_typeID);
  this->prepare(quitRequestCallback);
}

void MDK_EventLoopImpl_run(MDK_EventLoopImpl* this) {
  MDK_TypeID_ensure(this->id, MDK_EventLoopImpl_typeID);
  this->run();
}

void MDK_EventLoopImpl_sendEvent(MDK_EventLoopImpl* this, MDK_Event* event) {
  MDK_TypeID_ensure(this->id, MDK_EventLoopImpl_typeID);
  this->sendEvent(event);
}
