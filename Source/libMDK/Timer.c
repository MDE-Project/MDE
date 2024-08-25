#include <time.h>
#include <unistd.h>

#include <MDK/Application.h>
#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>
#include <MDK/Timer.h>

static uint64_t getMonotonicTime() {
  struct timespec timeSpec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &timeSpec);
  
  return ((uint64_t)timeSpec.tv_sec*1000000) + ((uint64_t)timeSpec.tv_nsec/1000);
}

static void timerOneShotMain(MDK_Object* this_raw) {
  CAST_THIS(MDK_Timer);
  
  uint64_t currentMonotonicTime = getMonotonicTime();
  if (currentMonotonicTime < this->nextTrigger) {
    usleep(this->nextTrigger - currentMonotonicTime);
  }
  
  MDK_Application_pause();
    MDK_Event* triggerEvent = MDK_Event_create(OBJ(this), this->triggerEventTarget, this->triggerEventHandler);
    MDK_Application_sendEvent(triggerEvent);
  MDK_Application_resume();
}

static void timerIntervalMain(MDK_Object* this_raw) {
  CAST_THIS(MDK_Timer);
  
  while (true) {
    if (getMonotonicTime() >= this->nextTrigger) {
      this->nextTrigger += this->interval;
      
      MDK_Application_pause();
        MDK_Event* triggerEvent = MDK_Event_create(OBJ(this), this->triggerEventTarget, this->triggerEventHandler);
        MDK_Application_sendEvent(triggerEvent);
      MDK_Application_resume();
    }
    
    uint64_t currentMonotonicTime = getMonotonicTime();
    if (currentMonotonicTime < this->nextTrigger) {
      usleep(this->nextTrigger - currentMonotonicTime);
    }
  }
}

MDK_Timer* MDK_Timer_create(MDK_Timer_Type type, uint64_t microseconds) {
  MDK_Timer* this = OBJ_CREATE(MDK_Timer);
  MDK_Timer_init(this, type, microseconds);
  return this;
}

void MDK_Timer_init(MDK_Timer* this, MDK_Timer_Type type, uint64_t microseconds) {
  this->id = MDK_Timer_typeID;
  this->type = type;
  this->interval = microseconds;
  this->timerTask = NULL;
  this->triggerEventTarget = NULL;
  this->triggerEventHandler = NULL;
}

void MDK_Timer_destroy(MDK_Timer* this) {
  ENSURE(MDK_Timer);
  UNREF_NULL(this->timerTask);
}

void MDK_Timer_start(MDK_Timer* this) {
  ENSURE(MDK_Timer);
  
  if (!MDK_Timer_getIsRunning(this)) {
    UNREF_NULL(this->timerTask);
    
    this->nextTrigger = getMonotonicTime() + this->interval;
    
    if (this->type == MDK_Timer_Type_oneShot) {
      this->timerTask = MDK_BackgroundTask_create(OBJ(this), timerOneShotMain);
    } else if (this->type == MDK_Timer_Type_interval) {
      this->timerTask = MDK_BackgroundTask_create(OBJ(this), timerIntervalMain);
    }
    
    REF(this->timerTask);
  }
}

void MDK_Timer_stop(MDK_Timer* this) {
  ENSURE(MDK_Timer);
  
  if (this->timerTask) {
    UNREF(this->timerTask);
    this->timerTask = NULL;
  }
}

bool MDK_Timer_getIsRunning(MDK_Timer* this) {
  ENSURE(MDK_Timer);
  
  if (!this->timerTask) {
    return false;
  } else {
    return MDK_BackgroundTask_getRunning(this->timerTask);
  }
}

void MDK_Timer_onTrigger(MDK_Timer* this, MDK_Object* target, MDK_Event_Handler handler) {
  ENSURE(MDK_Timer);
  
  if (!MDK_Timer_getIsRunning(this)) {
    this->triggerEventTarget = target;
    this->triggerEventHandler = handler;
  }
}
