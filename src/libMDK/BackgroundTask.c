#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>

#include <MDK/BackgroundTask.h>

struct MDK_BackgroundTask_struct {
  pthread_t thread;
  void (*main)(void* data);
  void* data;
  _Atomic bool isRunning;
};

static void* threadMain(void* task_raw) {
  MDK_BackgroundTask* task = task_raw;
  
  task->isRunning = true;
  pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
  task->main(task->data);
  
  task->isRunning = false;
  return NULL;
}

MDK_BackgroundTask* MDK_BackgroundTask_create(void (*taskMain)(void* data), void* data) {
  MDK_BackgroundTask* task = malloc(sizeof(MDK_BackgroundTask));
  atomic_init(&task->isRunning, false);
  
  task->main = taskMain;
  task->data = data;
  
  pthread_create(&task->thread, NULL, threadMain, task);
  pthread_detach(task->thread);
  
  return task;
}

void MDK_BackgroundTask_destroy(MDK_BackgroundTask* task) {
  if (!task->isRunning) {
    pthread_cancel(task->thread);
  }
  
  free(task);
}

bool MDK_BackgroundTask_getIsRunning(MDK_BackgroundTask* task) {
  return task->isRunning;
}

void MDK_BackgroundTask_stop(MDK_BackgroundTask* task) {
  if (!task->isRunning) {
    pthread_cancel(task->thread);
    task->isRunning = false;
  }
}
