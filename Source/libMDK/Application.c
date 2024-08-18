#include <stdlib.h>

#include <MDK/Application.h>
#include <MDK/Application/StartEvent.h>
#include <MDK/Event.h>
#include <MDK/EventLoop.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

static MDK_EventLoop* globalEventLoop;

static MDK_Object* startEventTarget = NULL;
static MDK_Application_StartEvent_Handler startEventHandler = NULL;

static void quitRequestEventDefaultHandler(MDK_Object* this, MDK_Event* event) {
  // FIXME Add some way to have the event loop itself return
  exit(0);
}

static MDK_Object* quitRequestEventTarget = NULL;
static MDK_Event_Handler quitRequestEventHandler = quitRequestEventDefaultHandler;

int MDK_Application_start(int argc, char** argv) {
  // FIXME
  return 0;
}

int MDK_Application_startWithEventLoop(int argc, char** argv, MDK_EventLoop* eventLoop) {
  globalEventLoop = eventLoop;
  REF(globalEventLoop);
  
  MDK_EventLoop_prepare(globalEventLoop, MDK_Application_requestQuit);
  
  MDK_Application_StartEvent* startEvent = MDK_Application_StartEvent_create(startEventTarget, startEventHandler, argc, argv);
  MDK_Application_sendEvent(EVT(startEvent));
  
  MDK_EventLoop_run(globalEventLoop);
  
  return 0;
}

void MDK_Application_sendEvent(MDK_Event* event) {
  MDK_EventLoop_sendEvent(globalEventLoop, event);
}

void MDK_Application_requestQuit() {
  MDK_Event* quitRequestEvent = MDK_Event_create(NULL, quitRequestEventTarget, quitRequestEventHandler);
  MDK_Application_sendEvent(quitRequestEvent);
}

void MDK_Application_onStart(MDK_Object* target, MDK_Application_StartEvent_Handler handler) {
  startEventTarget = target;
  startEventHandler = handler;
}

void MDK_Application_onQuitRequest(MDK_Object* target, MDK_Event_Handler handler) {
  quitRequestEventTarget = target;
  quitRequestEventHandler = handler;
}
