#include <stdio.h>
#include <stdlib.h>

#include <MDK/GenericSet.h>
#include <MDK/Object.h>
#include <MDK/Set.h>
#include <MDK/Shorthand.h>

void printHelp() {
  fputs("MDK manual test utility\n"
        "Usage: MDKTest <testNum>\n"
        "1 - Type signature checking\n"
        "2 - Set and object ref-counting test\n"
        "3 - GenericSet test\n", stdout);
}

void setRefTestDestructor(MDK_Object* this) {
  puts("Destructor called");
}

void setRefTest() {
  MDK_Set* set = MDK_Set_create(1);
  REF(set);
  
  for (unsigned i = 0; i < 4; i++) {
    MDK_Object* object = MDK_Object_create(setRefTestDestructor, 0);
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

int main(int argc, char** argv) {
  if (argc != 2) {
    printHelp();
    return 1;
  }
  
  int testNum = atoi(argv[1]);
  
  switch (testNum) {
    case 1:
      REF("Milk");
    break;
    case 2:
      setRefTest();
    break;
    case 3:
      genericSetTest();
    break;
    default:
      printHelp();
      return 1;
  }
}
