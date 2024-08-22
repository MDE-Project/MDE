#pragma once

#include <stdint.h>

#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/TypeID.h>

// FIXME: If you ever add a MDK_Time, change this
MDK_TypeID_create(MDK_Timer, 'T', 'i', 'm', 'e');

typedef enum {
  MDK_Timer_Type_oneShot,
  MDK_Timer_Type_interval,
} MDK_Timer_Type;

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  MDK_Timer_Type type;
  uint64_t interval;
  uint64_t nextTrigger;
  MDK_BackgroundTask* timerTask;
  MDK_Object* triggerEventTarget; // Not ref-counted
  MDK_Event_Handler triggerEventHandler;
} MDK_Timer;

MDK_Timer* MDK_Timer_create(MDK_Timer_Type type, uint64_t microseconds);
void MDK_Timer_init(MDK_Timer* this, MDK_Timer_Type type, uint64_t microseconds);
void MDK_Timer_destroy(MDK_Timer* this);

void MDK_Timer_start(MDK_Timer* this);
void MDK_Timer_stop(MDK_Timer* this);

bool MDK_Timer_getIsRunning(MDK_Timer* this);

void MDK_Timer_onTrigger(MDK_Timer* this, MDK_Object* target, MDK_Event_Handler handler);
