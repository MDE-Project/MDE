#include <stdlib.h>

#include <MDK/Application.h>
#include <MDK/RingBufferEventLoop.h>

static MDK_EventLoopImpl* globalEventLoop;

static void (*startEventTarget)(MDK_Application_StartEvent* event);

static void quitRequestEventDefaultTarget(MDK_Event* event) {
  exit(0);
}
static MDK_Event_Target quitRequestEventTarget = quitRequestEventDefaultTarget;

static void quitRequestReceived() {
  MDK_Event* quitRequestEvent = malloc(sizeof(MDK_Event));
  
  *quitRequestEvent = (MDK_Event){
    .target = quitRequestEventTarget,
    .callback = (MDK_Event_Target)free,
  };
  
  MDK_Application_sendEvent(quitRequestEvent);
}

int MDK_Application_startWithEventLoopImpl(int argc, char** argv, MDK_EventLoopImpl* eventLoop) {
  globalEventLoop = eventLoop;
  
  eventLoop->init(quitRequestReceived);
  
  MDK_Application_StartEvent* startEvent = malloc(sizeof(MDK_Application_StartEvent));
  
  *startEvent = (MDK_Application_StartEvent){
    .inherited = {
      .target = (MDK_Event_Target)startEventTarget,
      .callback = (MDK_Event_Target)free,
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

void MDK_Application_onStart(void (*target)(MDK_Application_StartEvent* event)) {
  startEventTarget = target;
}

void MDK_Application_onQuitRequest(MDK_Event_Target target) {
  quitRequestEventTarget = target;
}
