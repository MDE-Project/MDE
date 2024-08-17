#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>

#define MDK_Event_createHandlerType(type) \
  typedef void (*type##_Handler)(MDK_Object* this, type* event)

typedef struct MDK_Event_struct MDK_Event;
MDK_TypeID_create(MDK_Event, 'E', 'v', 'e', 'n');

MDK_Event_createHandlerType(MDK_Event);

struct MDK_Event_struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  MDK_Object* source;
  MDK_Object* target;
  MDK_Event_Handler handler;
};

MDK_Event* MDK_Event_create(MDK_Object* source, MDK_Object* target, MDK_Event_Handler handler);
void MDK_Event_init(MDK_Event* this, MDK_Object* source, MDK_Object* target, MDK_Event_Handler handler);
void MDK_Event_destroy(MDK_Event* this);
