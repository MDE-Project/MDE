#pragma once

#include <signal.h>
#include <pthread.h>

#include <MDK/BackgroundTask.h>
#include <MDK/EventLoop.h>
#include <MDK/Object.h>
#include <MDK/TypeID.h>

#define MDK_RingBufferEventLoop_ringSize (256)

MDK_TypeID_create(MDK_RingBufferEventLoop, 0x446317F2);

typedef struct {
  MDK_EventLoop inherited;
  MDK_TypeID id;
  
  pthread_mutex_t mainThreadMutex;
  pthread_cond_t eventReceivedCond;
  
  volatile unsigned ringReadOffset;
  volatile unsigned ringWriteOffset;
  
  MDK_BackgroundTask* signalTask;
  sigset_t signalSet;
  
  MDK_Event* volatile ring[MDK_RingBufferEventLoop_ringSize];
} MDK_RingBufferEventLoop;

MDK_RingBufferEventLoop* MDK_RingBufferEventLoop_create();
void MDK_RingBufferEventLoop_init(MDK_RingBufferEventLoop* this);
void MDK_RingBufferEventLoop_destroy(MDK_RingBufferEventLoop* this);
