#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <MDK/Application.h>
#include <MDK/BackgroundTask.h>

void backgroundEventsReceiver(MDK_Event* event) {
  puts("received my event");
  usleep(1000000);
}

void backgroundEventsTest(void* unused) {
  puts("hello from thread");
  
  for (unsigned i = 0; i < 10; i++) {
    MDK_Event* myEvent = malloc(sizeof(MDK_Event));
    *myEvent = (MDK_Event){
      .target = backgroundEventsReceiver,
      .callback = (MDK_Event_Target)free,
    };
    
    MDK_Application_sendEvent(myEvent);
    puts("sent an event");
  }
}

void onStart(MDK_Application_StartEvent* event) {
  puts("hello");
  
  MDK_BackgroundTask* backgroundEventsTask = MDK_BackgroundTask_create(backgroundEventsTest, NULL);
}

int main(int argc, char* argv[]) {
  MDK_Application_onStart(onStart);
  return MDK_Application_start(argc, argv);
}
