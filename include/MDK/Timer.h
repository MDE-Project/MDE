#pragma once

#include <stdbool.h>
#include <stdint.h>

#include <MDK/Event.h>

typedef struct MDK_Timer_struct MDK_Timer;

typedef enum {
  MDK_Timer_Type_oneShot,
  MDK_Timer_Type_interval,
} MDK_Timer_Type;

MDK_Timer* MDK_Timer_create(uint64_t microseconds, MDK_Timer_Type type);
void MDK_Timer_destroy(MDK_Timer* timer);

void MDK_Timer_start(MDK_Timer* timer);
void MDK_Timer_stop(MDK_Timer* timer);

bool MDK_Timer_getIsRunning(MDK_Timer* timer);

void MDK_Timer_onTrigger(MDK_Timer* timer, MDK_Event_Target target, void* data);
