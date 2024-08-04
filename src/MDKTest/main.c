#include <stdio.h>
#include <stdlib.h>

#include <MDK/Object.h>
#include <MDK/Set.h>
#include <MDK/Shorthand.h>

void printHelp() {
  fputs("MDK manual test utility\n"
        "Usage: MDKTest <testNum>\n"
        "1 - Type signature checking\n"
        "2 - Set and object ref-counting test\n", stdout);
}

void setRefTest() {
  MDK_Set* set = MDK_Set_create(1);
  REF(set);
  
  for (unsigned i = 0; i < 4; i++) {
    MDK_Object* object = MDK_Object_create(NULL, 0);
    MDK_Set_push(set, object);
  }
  
  puts("---");
  MDK_Set_remove(set, 1);
  puts("---");
  
  UNREF(set);
}

int main(int argc, char* argv[]) {
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
    default:
      printHelp();
      return 1;
  }
}
