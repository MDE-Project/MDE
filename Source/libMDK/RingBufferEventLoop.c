#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

#include <MDK/Application.h>
#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/EventLoop.h>
#include <MDK/EventLoop/StopEvent.h>
#include <MDK/Object.h>
#include <MDK/RingBufferEventLoop.h>
#include <MDK/Shorthand.h>

static _Thread_local bool isMainThread = false;

static void signalTaskMain(MDK_Object* this_raw) {
  CAST_THIS(MDK_RingBufferEventLoop);
  
  int signalReceived;
  
  while (true) {
    sigwait(&this->signalSet, &signalReceived);
    
    MDK_Application_pause();
      MDK_Application_requestQuit();
    MDK_Application_resume();
  }
}

static int run(MDK_EventLoop* this_raw) {
  CAST_THIS(MDK_RingBufferEventLoop);
  ENSURE(MDK_RingBufferEventLoop);
  
  pthread_sigmask(SIG_BLOCK, &this->signalSet, NULL);
  this->signalTask = MDK_BackgroundTask_create(OBJ(this), signalTaskMain);
  REF(this->signalTask);
  
  while (true) {
    pthread_mutex_lock(&this->mainThreadMutex);
    
    if (this->ring[this->ringReadOffset] == NULL) {
      pthread_cond_wait(&this->eventReceivedCond, &this->mainThreadMutex);
    }
    
    MDK_Event* volatile event = this->ring[this->ringReadOffset];
    
    if (MDK_Event_getStopEventLoop(event)) {
      return MDK_EventLoop_StopEvent_getExitCode((MDK_EventLoop_StopEvent*)event);
    }
    
    MDK_Event_dispatch(event);
    
    UNREF(event);
    this->ring[this->ringReadOffset] = NULL;
    this->ringReadOffset = (this->ringReadOffset+1) % MDK_RingBufferEventLoop_ringSize;
    
    pthread_mutex_unlock(&this->mainThreadMutex);
  }
}

static void pause(MDK_EventLoop* this_raw) {
  CAST_THIS(MDK_RingBufferEventLoop);
  ENSURE(MDK_RingBufferEventLoop);
  
  if (!isMainThread) {
    pthread_mutex_lock(&this->mainThreadMutex);
  }
}

static void resume(MDK_EventLoop* this_raw) {
  CAST_THIS(MDK_RingBufferEventLoop);
  ENSURE(MDK_RingBufferEventLoop);
  
  if (!isMainThread) {
    pthread_mutex_unlock(&this->mainThreadMutex);
  }
}

static void sendEvent(MDK_EventLoop* this_raw, MDK_Event* event) {
  CAST_THIS(MDK_RingBufferEventLoop);
  ENSURE(MDK_RingBufferEventLoop);
  
  this->ring[this->ringWriteOffset] = event;
  REF(event);
  this->ringWriteOffset = (this->ringWriteOffset+1) % MDK_RingBufferEventLoop_ringSize;
  
  if (this->ringWriteOffset == this->ringReadOffset) {
    fputs("MDK_RingBufferEventLoop: Event loop ring buffer full! Aborting!\n", stderr);
    abort();
  }
  
  if (!isMainThread) {
    pthread_cond_signal(&this->eventReceivedCond);
  }
}

MDK_RingBufferEventLoop* MDK_RingBufferEventLoop_create() {
  MDK_RingBufferEventLoop* this = OBJ_CREATE(MDK_RingBufferEventLoop);
  MDK_RingBufferEventLoop_init(this);
  return this;
}

void MDK_RingBufferEventLoop_init(MDK_RingBufferEventLoop* this) {
  MDK_EventLoop_init(&this->inherited, run, pause, resume, sendEvent);
  
  this->id = MDK_RingBufferEventLoop_typeID;
  
  pthread_mutex_init(&this->mainThreadMutex, NULL);
  pthread_cond_init(&this->eventReceivedCond, NULL);
  
  this->ringReadOffset = 0;
  this->ringWriteOffset = 0;
  
  this->signalTask = NULL;
  sigemptyset(&this->signalSet);
  sigaddset(&this->signalSet, SIGINT);
  sigaddset(&this->signalSet, SIGTERM);
  
  memset((void*)&this->ring, 0, sizeof(MDK_Event* volatile)*MDK_RingBufferEventLoop_ringSize);
  
  isMainThread = true;
}

void MDK_RingBufferEventLoop_destroy(MDK_RingBufferEventLoop* this) {
  ENSURE(MDK_RingBufferEventLoop);
  
  MDK_EventLoop_destroy(&this->inherited);
  
  pthread_mutex_destroy(&this->mainThreadMutex);
  pthread_cond_destroy(&this->eventReceivedCond);
  
  UNREF_NULL(this->signalTask);
  
  for (unsigned i = this->ringReadOffset; this->ring[i] != NULL; i = (i+1) % MDK_RingBufferEventLoop_ringSize) {
    UNREF(this->ring[i]);
    this->ring[i] = NULL;
  }
}
