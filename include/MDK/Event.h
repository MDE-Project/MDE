#pragma once

typedef struct MDK_Event_struct MDK_Event;

typedef void (*MDK_Event_Target)(MDK_Event* event, void* data);
typedef void (*MDK_Event_Callback)(MDK_Event* event);

struct MDK_Event_struct {
  MDK_Event_Target target;
  MDK_Event_Callback callback;
  void* data;
};
