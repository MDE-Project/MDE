#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/EventLoop.h>
#include <MDK/Object.h>
#include <MDK/RingBufferEventLoop.h>
#include <MDK/Shorthand.h>

static void signalTaskMain(MDK_Object* this_raw) {
  MDK_RingBufferEventLoop* this = (MDK_RingBufferEventLoop*)this_raw;
  
  int signalReceived;
  
  while (true) {
    sigwait(&this->signalSet, &signalReceived);
    this->inherited.quitRequestCallback();
  }
}

static void run(MDK_EventLoop* this_raw) {
  MDK_RingBufferEventLoop* this = (MDK_RingBufferEventLoop*)this_raw;
  
  pthread_sigmask(SIG_BLOCK, &this->signalSet, NULL);
  this->signalTask = MDK_BackgroundTask_create(OBJ(this), signalTaskMain);
  REF(this->signalTask);
  
  pthread_mutex_lock(&this->ringMutex);
  
  while (true) {
    if (this->ring[this->ringReadOffset] == NULL) {
      pthread_cond_wait(&this->ringCond, &this->ringMutex);
    }
    pthread_mutex_unlock(&this->ringMutex);
    
    MDK_Event* volatile event = this->ring[this->ringReadOffset];
    
    if (MDK_Event_getStopEventLoop(event)) {
      return;
    }
    
    MDK_Event_deliver(event);
    
    UNREF(event);
    this->ring[this->ringReadOffset] = NULL;
    
    pthread_mutex_lock(&this->ringMutex);
    this->ringReadOffset = (this->ringReadOffset+1) % MDK_RingBufferEventLoop_ringSize;
  }
}

static void sendEvent(MDK_EventLoop* this_raw, MDK_Event* event) {
  MDK_RingBufferEventLoop* this = (MDK_RingBufferEventLoop*)this_raw;
  
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  pthread_mutex_lock(&this->ringMutex);
  
  this->ring[this->ringWriteOffset] = event;
  REF(event);
  this->ringWriteOffset = (this->ringWriteOffset+1) % MDK_RingBufferEventLoop_ringSize;
  
  if (this->ringWriteOffset == this->ringReadOffset) {
    fputs("Event loop ring buffer full! Aborting!\n", stderr);
    abort();
  }
  
  pthread_cond_broadcast(&this->ringCond);
  pthread_mutex_unlock(&this->ringMutex);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

MDK_RingBufferEventLoop* MDK_RingBufferEventLoop_create() {
  MDK_RingBufferEventLoop* this = OBJ_CREATE(MDK_RingBufferEventLoop);
  MDK_RingBufferEventLoop_init(this);
  return this;
}

void MDK_RingBufferEventLoop_init(MDK_RingBufferEventLoop* this) {
  MDK_EventLoop_init(&this->inherited, run, sendEvent);
  
  this->id = MDK_RingBufferEventLoop_typeID;
  
  pthread_mutex_init(&this->ringMutex, NULL);
  pthread_cond_init(&this->ringCond, NULL);
  this->ringReadOffset = 0;
  this->ringWriteOffset = 0;
  
  this->signalTask = NULL;
  sigemptyset(&this->signalSet);
  sigaddset(&this->signalSet, SIGINT);
  sigaddset(&this->signalSet, SIGTERM);
  
  memset((void*)&this->ring, 0, sizeof(MDK_Event* volatile)*MDK_RingBufferEventLoop_ringSize);
}

void MDK_RingBufferEventLoop_destroy(MDK_RingBufferEventLoop* this) {
  MDK_TypeID_ensure(this->id, MDK_RingBufferEventLoop_typeID);
  
  MDK_EventLoop_destroy(&this->inherited);
  
  pthread_mutex_destroy(&this->ringMutex);
  pthread_cond_destroy(&this->ringCond);
  
  if (this->signalTask) {
    UNREF(this->signalTask);
  }
  
  for (unsigned i = this->ringReadOffset; this->ring[i] != NULL; i = (i+1) % MDK_RingBufferEventLoop_ringSize) {
    UNREF(this->ring[i]);
    this->ring[i] = NULL;
  }
}
