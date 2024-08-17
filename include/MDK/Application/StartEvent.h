#pragma once

#include <MDK/Event.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_Application_StartEvent, 'A', 'S', 'E', 'v');

typedef struct {
  MDK_Event inherited;
  MDK_TypeID id;
  
  // FIXME: Replace with an MDK_Array when I make it
  int argc;
  char** argv;
} MDK_Application_StartEvent;

MDK_Event_createHandlerType(MDK_Application_StartEvent);

MDK_Application_StartEvent* MDK_Application_StartEvent_create(MDK_Object* target, MDK_Application_StartEvent_Handler handler, int argc, char** argv);
void MDK_Application_StartEvent_init(MDK_Application_StartEvent* this, MDK_Object* target, MDK_Application_StartEvent_Handler handler, int argc, char** argv);
void MDK_Application_StartEvent_destroy(MDK_Application_StartEvent* this);
