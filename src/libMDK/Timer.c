#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <MDK/Application.h>
#include <MDK/BackgroundTask.h>
#include <MDK/Timer.h>

struct MDK_Timer_struct {
  MDK_Timer_Type type;
  
  uint64_t interval;
  uint64_t nextTrigger;
  
  MDK_BackgroundTask* timerTask;
  MDK_Event_Target triggerEventTarget;
  void* triggerEventData;
};

static uint64_t getMonotonicTime() {
  struct timespec timeSpec;
  clock_gettime(CLOCK_MONOTONIC_RAW, &timeSpec);
  
  return (uint64_t)timeSpec.tv_sec*1000000 + (uint64_t)timeSpec.tv_nsec/1000;
}

static void timerOneShotMain(void* timer_raw) {
  MDK_Timer* timer = timer_raw;
  
  uint64_t currentMonotonicTime = getMonotonicTime();
  if (currentMonotonicTime > timer->nextTrigger) {
    usleep(timer->nextTrigger - currentMonotonicTime);
  }
  
  MDK_Application_sendEvent(NULL, timer->triggerEventTarget, timer->triggerEventData, (MDK_Event_Callback)free);
}

static void timerIntervalMain(void* timer_raw) {
  MDK_Timer* timer = timer_raw;
  
  while (true) {
    if (getMonotonicTime() >= timer->nextTrigger) {
      timer->nextTrigger += timer->interval;
      
      MDK_Application_sendEvent(NULL, timer->triggerEventTarget, timer->triggerEventData, (MDK_Event_Callback)free);
    }
    
    uint64_t currentMonotonicTime = getMonotonicTime();
    if (currentMonotonicTime > timer->nextTrigger) {
      usleep(timer->nextTrigger - currentMonotonicTime);
    }
  }
}

MDK_Timer* MDK_Timer_create(uint64_t microseconds, MDK_Timer_Type type) {
  MDK_Timer* timer = malloc(sizeof(MDK_Timer));
  
  timer->type = type;
  timer->interval = microseconds;
  timer->timerTask = NULL;
  timer->triggerEventTarget = NULL;
  
  return timer;
}

void MDK_Timer_destroy(MDK_Timer* timer) {
  if (timer->timerTask) {
    MDK_BackgroundTask_destroy(timer->timerTask);
  }
  
  free(timer);
}

void MDK_Timer_start(MDK_Timer* timer) {
  if (!MDK_Timer_getIsRunning(timer)) {
    if (timer->timerTask) {
      MDK_BackgroundTask_destroy(timer->timerTask);
    }
    
    timer->nextTrigger = getMonotonicTime() + timer->interval;
    
    if (timer->type == MDK_Timer_Type_oneShot) {
      timer->timerTask = MDK_BackgroundTask_create(timerOneShotMain, timer);
    } else if (timer->type == MDK_Timer_Type_interval) {
      timer->timerTask = MDK_BackgroundTask_create(timerIntervalMain, timer);
    }
  }
}

void MDK_Timer_stop(MDK_Timer* timer) {
  if (MDK_Timer_getIsRunning(timer)) {
    MDK_BackgroundTask_destroy(timer->timerTask);
    timer->timerTask = NULL;
  }
}

bool MDK_Timer_getIsRunning(MDK_Timer* timer) {
  if (!timer->timerTask) {
    return false;
  } else {
    return MDK_BackgroundTask_getIsRunning(timer->timerTask);
  }
}

void MDK_Timer_onTrigger(MDK_Timer* timer, MDK_Event_Target target, void* data) {
  if (!MDK_Timer_getIsRunning(timer)) {
    timer->triggerEventTarget = target;
    timer->triggerEventData = data;
  }
}
