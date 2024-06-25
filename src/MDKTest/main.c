#include <stdio.h>

#include <MDK/Application.h>

void onStart(MDK_Application_StartEvent* event) {
  puts("hello");
}

int main(int argc, char* argv[]) {
  MDK_Application_onStart(onStart);
  return MDK_Application_start(argc, argv);
}
