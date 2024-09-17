#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_Event* MDK_Event_create(MDK_Object* source, MDK_Object* target, MDK_Event_Handler handler) {
  MDK_Event* this = OBJ_CREATE(MDK_Event);
  MDK_Event_init(this, source, target, handler);
  return this;
}

void MDK_Event_init(MDK_Event* this, MDK_Object* source, MDK_Object* target, MDK_Event_Handler handler) {
  this->id = MDK_Event_typeID;
  this->source = source;
  REF_NULL(source);
  this->target = target;
  REF_NULL(target);
  this->handler = handler;
  this->stopEventLoop = false;
}

void MDK_Event_destroy(MDK_Event* this) {
  ENSURE(MDK_Event);
  
  UNREF_NULL(this->source);
  UNREF_NULL(this->target);
}

void MDK_Event_dispatch(MDK_Event* this) {
  ENSURE(MDK_Event);
  
  this->handler(this->target, this);
}

bool MDK_Event_getStopEventLoop(MDK_Event* this) {
  ENSURE(MDK_Event);
  return this->stopEventLoop;
}
