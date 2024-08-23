#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <MDK/Application.h>
#include <MDK/Application/StartEvent.h>
#include <MDK/BackgroundTask.h>
#include <MDK/Event.h>
#include <MDK/GenericArray.h>
#include <MDK/GenericSet.h>
#include <MDK/Object.h>
#include <MDK/Set.h>
#include <MDK/Shorthand.h>
#include <MDK/Timer.h>

void printHelp() {
  fputs("MDK manual test utility\n"
        "Usage: MDKTest <testNum>\n"
        "1 - Type ID checking\n"
        "2 - Set and object ref-counting test\n"
        "3 - GenericSet test\n"
        "4 - Inherited event test\n"
        "5 - Basic application test\n"
        "6 - Timer test\n"
        "7 - GenericArray test\n", stdout);
}

void dummyDestructor(MDK_Object* this) {
  puts("Destructor called");
}

void typeIDTest() {
  REF("Milk");
}

void setRefTest() {
  MDK_Set* set = MDK_Set_create(1);
  REF(set);
  
  for (unsigned i = 0; i < 4; i++) {
    MDK_Object* object = MDK_Object_create(dummyDestructor, 0);
    MDK_Set_push(set, object);
  }
  
  puts("---");
  MDK_Set_remove(set, 1);
  puts("---");
  
  UNREF(set);
}

void genericSetTestPrint(MDK_GenericSet* set) {
  unsigned* array = MDK_GenericSet_getRawArray(set);
  unsigned length = MDK_GenericSet_getLength(set);
  
  for (unsigned i = 0; i < length; i++) {
    printf("%u\n", array[i]);
  }
}

void genericSetTest() {
  MDK_GenericSet* set = MDK_GenericSet_create(sizeof(unsigned), 1);
  REF(set);
  unsigned* array = MDK_GenericSet_getRawArray(set);
  
  unsigned item;
  
  item = 1;
  MDK_GenericSet_push(set, &item);
  item = 2;
  MDK_GenericSet_push(set, &item);
  item = 3;
  MDK_GenericSet_push(set, &item);
  genericSetTestPrint(set);
  
  puts("---");
  
  array[1] = 4;
  genericSetTestPrint(set);
  
  puts("---");
  
  item = 2;
  MDK_GenericSet_push(set, &item);
  genericSetTestPrint(set);
  
  puts("---");
  
  MDK_GenericSet_remove(set, 0);
  genericSetTestPrint(set);
  
  puts("---");
  
  MDK_GenericSet_remove(set, 1);
  genericSetTestPrint(set);
  
  UNREF(set);
}

void inheritedEventTest(int argc, char** argv) {
  MDK_Set* set = MDK_Set_create(4);
  
  for (unsigned i = 0; i < 4; i++) {
    MDK_Object* object = MDK_Object_create(dummyDestructor, 0);
    MDK_Set_push(set, object);
  }
  
  MDK_Application_StartEvent* startEvent = MDK_Application_StartEvent_create(OBJ(set), (void*)0x1234, argc, argv);
  REF(startEvent);
  
  printf("%p %i %s\n", startEvent->inherited.handler, startEvent->argc, startEvent->argv[1]);
  
  UNREF(startEvent);
}

MDK_BackgroundTask* basicApplicationTestTask;

void basicApplicationTestEventHandler(MDK_Object* unused, MDK_Event* event) {
  puts("Received event from task");
}

void basicApplicationTestTaskMain(MDK_Object* unused) {
  for (unsigned i = 0; i < 5; i++) {
    puts("Sending event from task");
    
    MDK_Application_pause();
      MDK_Event* testEvent = MDK_Event_create(NULL, NULL, basicApplicationTestEventHandler);
      MDK_Application_sendEvent(testEvent);
    MDK_Application_resume();
    
    sleep(1);
  }
}

void basicApplicationTest(MDK_Object* unused, MDK_Application_StartEvent* event) {
  puts("Hello from application");
  
  basicApplicationTestTask = MDK_BackgroundTask_create(NULL, basicApplicationTestTaskMain);
  REF(basicApplicationTestTask);
}

void basicApplicationTestQuit(MDK_Object* unused, MDK_Event* event) {
  puts("Bye!");
  UNREF(basicApplicationTestTask);
  MDK_Application_quit(0);
}

MDK_Timer* timerTestOneShot;
MDK_Timer* timerTestInterval;

void timerTestOneShotHandler(MDK_Object* unused, MDK_Event* event) {
  puts("One shot timer event");
}

void timerTestIntervalHandler(MDK_Object* unused, MDK_Event* event) {
  puts("Interval timer event");
}

void timerTest(MDK_Object* unused, MDK_Application_StartEvent* event) {
  timerTestOneShot = MDK_Timer_create(MDK_Timer_Type_oneShot, 2000000);
  REF(timerTestOneShot);
  MDK_Timer_onTrigger(timerTestOneShot, NULL, timerTestOneShotHandler);
  MDK_Timer_start(timerTestOneShot);
  
  timerTestInterval = MDK_Timer_create(MDK_Timer_Type_interval, 1000000);
  REF(timerTestInterval);
  MDK_Timer_onTrigger(timerTestInterval, NULL, timerTestIntervalHandler);
  MDK_Timer_start(timerTestInterval);
}

void timerTestQuit(MDK_Object* unused, MDK_Event* event) {
  puts("Bye!");
  UNREF(timerTestOneShot);
  UNREF(timerTestInterval);
  MDK_Application_quit(0);
}

void genericArrayTestPrint(MDK_GenericArray* array) {
  unsigned* rawArray = MDK_GenericArray_getRawArray(array);
  unsigned length = MDK_GenericArray_getLength(array);
  
  for (unsigned i = 0; i < length; i++) {
    printf("%u\n", rawArray[i]);
  }
}

void genericArrayTest() {
  MDK_GenericArray* array = MDK_GenericArray_create(sizeof(unsigned), 1);
  REF(array);
  unsigned* rawArray = MDK_GenericArray_getRawArray(array);
  
  unsigned item;
  
  item = 1;
  MDK_GenericArray_push(array, &item);
  item = 2;
  MDK_GenericArray_push(array, &item);
  item = 3;
  MDK_GenericArray_push(array, &item);
  genericArrayTestPrint(array);
  
  puts("---");
  
  rawArray[1] = 4;
  genericArrayTestPrint(array);
  
  puts("---");
  
  item = 2;
  MDK_GenericArray_push(array, &item);
  genericArrayTestPrint(array);
  
  puts("---");
  
  MDK_GenericArray_remove(array, 0);
  genericArrayTestPrint(array);
  
  puts("---");
  
  MDK_GenericArray_remove(array, 1);
  genericArrayTestPrint(array);
  
  UNREF(array);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printHelp();
    return 1;
  }
  
  int testNum = atoi(argv[1]);
  
  switch (testNum) {
    case 1:
      typeIDTest();
    break;
    case 2:
      setRefTest();
    break;
    case 3:
      genericSetTest();
    break;
    case 4:
      inheritedEventTest(argc, argv);
    break;
    case 5:
      MDK_Application_onStart(NULL, basicApplicationTest);
      MDK_Application_onQuitRequest(NULL, basicApplicationTestQuit);
      return MDK_Application_start(argc, argv);
    case 6:
      MDK_Application_onStart(NULL, timerTest);
      MDK_Application_onQuitRequest(NULL, timerTestQuit);
      return MDK_Application_start(argc, argv);
    case 7:
      genericArrayTest();
      break;
    default:
      printHelp();
      return 1;
  }
}
