#include <stdlib.h>

#include <MDK/Application.h>
#include <MDK/RingBufferEventLoop.h>

static MDK_EventLoopImpl* globalEventLoop;

static MDK_Application_StartEvent_Target startEventTarget;
static void* startEventData;

static void quitRequestEventDefaultTarget(MDK_Event* event, void* data) {
  exit(0);
}
static MDK_Event_Target quitRequestEventTarget = quitRequestEventDefaultTarget;
static void* quitRequestEventData;

int MDK_Application_startWithEventLoopImpl(int argc, char** argv, MDK_EventLoopImpl* eventLoop) {
  globalEventLoop = eventLoop;
  
  eventLoop->init(MDK_Application_quit);
  
  MDK_Application_StartEvent* startEvent = malloc(sizeof(MDK_Application_StartEvent));
  
  *startEvent = (MDK_Application_StartEvent){
    .inherited = {
      .target = (MDK_Event_Target)startEventTarget,
      .data = startEventData,
      .callback = (MDK_Event_Callback)free,
    },
    .argc = argc,
    .argv = argv,
  };
  
  eventLoop->sendEvent((MDK_Event*)startEvent);
  
  eventLoop->run();
  
  return 0;
}

int MDK_Application_start(int argc, char** argv) {
  MDK_EventLoopImpl eventLoop = MDK_RingBufferEventLoop_create();
  return MDK_Application_startWithEventLoopImpl(argc, argv, &eventLoop);
}

void MDK_Application_sendEvent(MDK_Event* event) {
  globalEventLoop->sendEvent(event);
}

void MDK_Application_quit() {
  MDK_Event* quitRequestEvent = malloc(sizeof(MDK_Event));
  
  *quitRequestEvent = (MDK_Event){
    .target = quitRequestEventTarget,
    .data = quitRequestEventData,
    .callback = (MDK_Event_Callback)free,
  };
  
  MDK_Application_sendEvent(quitRequestEvent);
}

void MDK_Application_onStart(MDK_Application_StartEvent_Target target, void* data) {
  startEventTarget = target;
  startEventData = data;
}

void MDK_Application_onQuitRequest(MDK_Event_Target target, void* data) {
  quitRequestEventTarget = target;
  quitRequestEventData = data;
}
