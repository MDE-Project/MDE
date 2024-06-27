#include <stdio.h>
#include <stdlib.h>

#include <MDK/Application.h>
#include <MDK/Timer.h>

MDK_Timer* myTimer;
unsigned tickCount = 0;

void onTrigger(MDK_Event* event) {
  puts("Timer has ticked");
  
  tickCount++;
  if (tickCount == 5) {
    MDK_Timer_stop(myTimer);
    MDK_Application_quit();
  }
}

void onStart(MDK_Application_StartEvent* event) {
  puts("Application has started");
  printf("My name is %s and I have %i arguments\n", event->argv[0], event->argc);
  
  myTimer = MDK_Timer_create(1000000, MDK_Timer_Type_interval);
  MDK_Timer_onTrigger(myTimer, onTrigger);
  MDK_Timer_start(myTimer);
}

void onQuitRequest(MDK_Event* event) {
  puts("Bye!");
  exit(0);
}

int main(int argc, char* argv[]) {
  MDK_Application_onStart(onStart);
  MDK_Application_onQuitRequest(onQuitRequest);
  return MDK_Application_start(argc, argv);
}
