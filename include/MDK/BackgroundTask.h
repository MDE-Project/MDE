#pragma once

#include <pthread.h>
#include <stdbool.h>

#include <MDK/Object.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_BackgroundTask, 'B', 'T', 'a', 's');

typedef void (*MDK_BackgroundTask_MainFunc)(MDK_Object* this);

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  pthread_t thread;
  MDK_BackgroundTask_MainFunc main;
  MDK_Object* owner; // Not ref-counted
  _Atomic bool running;
} MDK_BackgroundTask;

MDK_BackgroundTask* MDK_BackgroundTask_create(MDK_Object* owner, MDK_BackgroundTask_MainFunc threadMain);
void MDK_BackgroundTask_init(MDK_BackgroundTask* this, MDK_Object* owner, MDK_BackgroundTask_MainFunc threadMain);
void MDK_BackgroundTask_destroy(MDK_BackgroundTask* this);

void MDK_BackgroundTask_stop(MDK_BackgroundTask* this);

bool MDK_BackgroundTask_getRunning(MDK_BackgroundTask* this);
