#include <stddef.h>

#include <MDK/Application.h>
#include <MTK/WindowManager.h>

void appMain(MDK_Application_StartEvent* event, void* unused) {
  MTK_WindowManager* windowManager = MTK_WindowManager_create();
}

int main(int argc, char** argv) {
  MDK_Application_onStart(appMain, NULL);
  return MDK_Application_start(argc, argv);
}
