#include <MDK/Application.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

#include "MTKTest.h"

static void start(MDK_Object* unused, MDK_Application_StartEvent* event) {
  MTKTest* mainObject = MTKTest_create();
  MDK_Application_setMainObject(OBJ(mainObject));
}

int main(int argc, char** argv) {
  MDK_Application_onStart(NULL, start);
  return MDK_Application_start(argc, argv);
}
