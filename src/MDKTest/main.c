#include <stdio.h>
#include <stdlib.h>

#include <MDK/Application.h>

unsigned quitRequestCount = 0;

void onStart(MDK_Application_StartEvent* event) {
  puts("Application has started");
  printf("My name is %s and I have %i arguments\n", event->argv[0], event->argc);
}

void onQuitRequest(MDK_Event* event) {
  quitRequestCount++;
  
  printf("Received quit request no. %u\n", quitRequestCount);
  
  if (quitRequestCount == 5) {
    puts("Fine! Quitting");
    exit(0);
  }
}

int main(int argc, char* argv[]) {
  MDK_Application_onStart(onStart);
  MDK_Application_onQuitRequest(onQuitRequest);
  return MDK_Application_start(argc, argv);
}
