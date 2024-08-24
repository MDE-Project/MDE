#include <MDK/Application/StartEvent.h>
#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_Application_StartEvent* MDK_Application_StartEvent_create(MDK_Object* target, MDK_Application_StartEvent_Handler handler, int argc, char** argv) {
  MDK_Application_StartEvent* this = OBJ_CREATE(MDK_Application_StartEvent);
  MDK_Application_StartEvent_init(this, target, handler, argc, argv);
  return this;
}

void MDK_Application_StartEvent_init(MDK_Application_StartEvent* this, MDK_Object* target, MDK_Application_StartEvent_Handler handler, int argc, char** argv) {
  MDK_Event_init(&this->inherited, NULL, target, (MDK_Event_Handler)handler);
  this->id = MDK_Application_StartEvent_typeID;
  this->argc = argc;
  this->argv = argv;
}

void MDK_Application_StartEvent_destroy(MDK_Application_StartEvent* this) {
  ENSURE(MDK_Application_StartEvent);
  MDK_Event_destroy(&this->inherited);
}
