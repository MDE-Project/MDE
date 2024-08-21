#pragma once

#include <MDK/Application/StartEvent.h>
#include <MDK/Event.h>
#include <MDK/EventLoop.h>

int MDK_Application_start(int argc, char** argv);
int MDK_Application_startWithEventLoop(int argc, char** argv, MDK_EventLoop* eventLoop);

void MDK_Application_pause();
void MDK_Application_resume();
void MDK_Application_sendEvent(MDK_Event* event);
void MDK_Application_quit(int exitCode);
void MDK_Application_requestQuit();

void MDK_Application_onStart(MDK_Object* target, MDK_Application_StartEvent_Handler handler);
void MDK_Application_onQuitRequest(MDK_Object* target, MDK_Event_Handler handler);
