#pragma once

#include <stdbool.h>

#include <MDK/Object.h>
#include <MDK/TypeID.h>

#define MDK_Event_createHandlerType(TYPE) \
  typedef void (*TYPE##_Handler)(MDK_Object* this, TYPE* event)

typedef struct MDK_Event_struct MDK_Event;
MDK_TypeID_create(MDK_Event, 0x1C5DF485);

MDK_Event_createHandlerType(MDK_Event);

struct MDK_Event_struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  bool stopEventLoop;
  
  MDK_Object* source;
  MDK_Object* target;
  MDK_Event_Handler handler;
};

MDK_Event* MDK_Event_create(MDK_Object* source, MDK_Object* target, MDK_Event_Handler handler);
void MDK_Event_init(MDK_Event* this, MDK_Object* source, MDK_Object* target, MDK_Event_Handler handler);
void MDK_Event_destroy(MDK_Event* this);

void MDK_Event_dispatch(MDK_Event* this);

bool MDK_Event_getStopEventLoop(MDK_Event* this);
