#include <MDK/Event.h>
#include <MDK/EventLoop/StopEvent.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_EventLoop_StopEvent* MDK_EventLoop_StopEvent_create() {
  MDK_EventLoop_StopEvent* this = OBJ_CREATE(MDK_EventLoop_StopEvent);
  MDK_EventLoop_StopEvent_init(this);
  return this;
}

void MDK_EventLoop_StopEvent_init(MDK_EventLoop_StopEvent* this) {
  MDK_Event_init(&this->inherited, NULL, NULL, NULL);
  this->id = MDK_EventLoop_StopEvent_typeID;
  this->inherited.stopEventLoop = true;
}

void MDK_EventLoop_StopEvent_destroy(MDK_EventLoop_StopEvent* this) {
  MDK_TypeID_ensure(this->id, MDK_EventLoop_StopEvent_typeID);
  MDK_Event_destroy(&this->inherited);
}
