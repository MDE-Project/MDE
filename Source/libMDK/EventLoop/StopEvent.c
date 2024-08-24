#include <MDK/Event.h>
#include <MDK/EventLoop/StopEvent.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_EventLoop_StopEvent* MDK_EventLoop_StopEvent_create(int exitCode) {
  MDK_EventLoop_StopEvent* this = OBJ_CREATE(MDK_EventLoop_StopEvent);
  MDK_EventLoop_StopEvent_init(this, exitCode);
  return this;
}

void MDK_EventLoop_StopEvent_init(MDK_EventLoop_StopEvent* this, int exitCode) {
  MDK_Event_init(&this->inherited, NULL, NULL, NULL);
  this->id = MDK_EventLoop_StopEvent_typeID;
  this->exitCode = exitCode;
  
  this->inherited.stopEventLoop = true;
}

void MDK_EventLoop_StopEvent_destroy(MDK_EventLoop_StopEvent* this) {
  ENSURE(MDK_EventLoop_StopEvent);
  MDK_Event_destroy(&this->inherited);
}

int MDK_EventLoop_StopEvent_getExitCode(MDK_EventLoop_StopEvent* this) {
  ENSURE(MDK_EventLoop_StopEvent);
  return this->exitCode;
}
