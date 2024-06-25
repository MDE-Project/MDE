#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

#include <MDK/BackgroundTask.h>

struct MDK_BackgroundTask_struct {
  pthread_t thread;
  void (*main)(void* data);
  void* data;
};

static void* threadMain(void* task_raw) {
  MDK_BackgroundTask* task = task_raw;
  
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  
  task->main(task->data);
  return NULL;
}

MDK_BackgroundTask* MDK_BackgroundTask_create(void (*taskMain)(void* data), void* data) {
  MDK_BackgroundTask* task = malloc(sizeof(MDK_BackgroundTask));
  
  task->main = taskMain;
  task->data = data;
  
  pthread_create(&task->thread, NULL, threadMain, task);
  pthread_detach(task->thread);
  
  return task;
}

void MDK_BackgroundTask_destroy(MDK_BackgroundTask* task) {
  pthread_cancel(task->thread);
  free(task);
}

void MDK_BackgroundTask_stop(MDK_BackgroundTask* task) {
  pthread_cancel(task->thread);
}
