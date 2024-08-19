#include <pthread.h>
#include <stdatomic.h>

#include <MDK/BackgroundTask.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

static void* taskStart(void* this_raw) {
  MDK_BackgroundTask* this = this_raw;
  
  this->running = true;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  this->main(this->owner);
  
  this->running = false;
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
  pthread_detach(this->thread);
}

void MDK_BackgroundTask_destroy(MDK_BackgroundTask* this) {
  MDK_TypeID_ensure(this->id, MDK_BackgroundTask_typeID);
  
  if (this->running) {
    pthread_cancel(this->thread);
  }
}

void MDK_BackgroundTask_stop(MDK_BackgroundTask* this) {
  MDK_TypeID_ensure(this->id, MDK_BackgroundTask_typeID);
  
  if (this->running) {
    pthread_cancel(this->thread);
    this->running = false;
  }
}

bool MDK_BackgroundTask_getRunning(MDK_BackgroundTask* this) {
  MDK_TypeID_ensure(this->id, MDK_BackgroundTask_typeID);
  return this->running;
}
