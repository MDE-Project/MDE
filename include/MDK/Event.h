#pragma once

#define MDK_Event_createTargetType(name) \
  typedef void (*name##_Target)(name* event, void* data)

typedef struct MDK_Event_struct MDK_Event;

MDK_Event_createTargetType(MDK_Event);
typedef void (*MDK_Event_Callback)(MDK_Event* event);

struct MDK_Event_struct {
  MDK_Event_Target target;
  MDK_Event_Callback callback;
  void* data;
};
