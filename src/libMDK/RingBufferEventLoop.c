#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include <MDK/BackgroundTask.h>
#include <MDK/RingBufferEventLoop.h>

#define EVENT_RING_SIZE (8192)

static pthread_mutex_t eventRingMutex;
static pthread_cond_t eventRingCond;

static MDK_Event* volatile eventRing[EVENT_RING_SIZE] = { NULL };
static volatile unsigned eventRingReadOffset = 0;
static volatile unsigned eventRingWriteOffset = 0;

static sigset_t signalSet;
static void (*quitRequestReceived)();

static void signalReceiver(void* unused) {
  int signalReceived;
  
  while (true) {
    sigwait(&signalSet, &signalReceived);
    quitRequestReceived();
  }
}

static void eventLoopInit(void (*quitRequestCallback)()) {
  quitRequestReceived = quitRequestCallback;
  
  pthread_mutex_init(&eventRingMutex, NULL);
  pthread_cond_init(&eventRingCond, NULL);
  
  sigemptyset(&signalSet);
  sigaddset(&signalSet, SIGINT);
  sigaddset(&signalSet, SIGTERM);
  
  pthread_sigmask(SIG_BLOCK, &signalSet, NULL);
}

static void eventLoopRun() {
  MDK_BackgroundTask_create(signalReceiver, NULL);
  
  pthread_mutex_lock(&eventRingMutex);
  
  while (true) {
    if (eventRing[eventRingReadOffset] == NULL) {
      pthread_cond_wait(&eventRingCond, &eventRingMutex);
    }
    
    pthread_mutex_unlock(&eventRingMutex);
    
    MDK_Event* volatile event = eventRing[eventRingReadOffset];
    
    event->target(event, event->data);
    event->callback(event);
    
    eventRing[eventRingReadOffset] = NULL;
    
    pthread_mutex_lock(&eventRingMutex);
    eventRingReadOffset = (eventRingReadOffset+1) % EVENT_RING_SIZE;
  }
}

static void eventLoopSendEvent(MDK_Event* event) {
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  pthread_mutex_lock(&eventRingMutex);
  
  eventRing[eventRingWriteOffset] = event;
  eventRingWriteOffset = (eventRingWriteOffset+1) % EVENT_RING_SIZE;
  
  if (eventRingWriteOffset == eventRingReadOffset) {
    fputs("Event loop ring buffer full! Aborting!\n", stderr);
    abort();
  }
  
  pthread_cond_broadcast(&eventRingCond);
  pthread_mutex_unlock(&eventRingMutex);
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
}

MDK_EventLoopImpl MDK_RingBufferEventLoop_create() {
  return (MDK_EventLoopImpl){
    .init = eventLoopInit,
    .run = eventLoopRun,
    .sendEvent = eventLoopSendEvent,
  };
}
