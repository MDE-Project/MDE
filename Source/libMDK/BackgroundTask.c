#include <pthread.h>
#include <stdatomic.h>

#include <MDK/Application.h>
#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

static void joinThread(MDK_Object* this_raw, MDK_Event* event) {
  CAST_THIS(MDK_BackgroundTask);
  pthread_join(this->thread, NULL);
}

static void* taskStart(void* this_raw) {
  CAST_THIS(MDK_BackgroundTask);
  
  this->running = true;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  this->main(this->owner);
  
  this->running = false;
  
  MDK_Application_pause();
    MDK_Event* event = MDK_Event_create(NULL, OBJ(this), joinThread);
    MDK_Application_sendEvent(event);
  MDK_Application_resume();
  
  return NULL;
}

MDK_BackgroundTask* MDK_BackgroundTask_create(MDK_Object* owner, MDK_BackgroundTask_MainFunc threadMain) {
  MDK_BackgroundTask* this = OBJ_CREATE(MDK_BackgroundTask);
  MDK_BackgroundTask_init(this, owner, threadMain);
  return this;
}

void MDK_BackgroundTask_init(MDK_BackgroundTask* this, MDK_Object* owner, MDK_BackgroundTask_MainFunc threadMain) {
  this->id = MDK_BackgroundTask_typeID;
  this->main = threadMain;
  this->owner = owner;
  atomic_init(&this->running, false);  
  
  pthread_create(&this->thread, NULL, taskStart, this);
}

void MDK_BackgroundTask_destroy(MDK_BackgroundTask* this) {
  ENSURE(MDK_BackgroundTask);
  
  if (this->running) {
    pthread_cancel(this->thread);
    pthread_join(this->thread, NULL);
  }
}

void MDK_BackgroundTask_stop(MDK_BackgroundTask* this) {
  ENSURE(MDK_BackgroundTask);
  
  if (this->running) {
    pthread_cancel(this->thread);
    pthread_join(this->thread, NULL);
    this->running = false;
  }
}

bool MDK_BackgroundTask_getRunning(MDK_BackgroundTask* this) {
  ENSURE(MDK_BackgroundTask);
  return this->running;
}
