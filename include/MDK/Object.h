#pragma once

#include <stddef.h>

#include <MDK/TypeID.h>

typedef struct MDK_Object_struct MDK_Object;
MDK_TypeID_create(MDK_Object, 0x6E719025);

typedef void (*MDK_Object_Destructor)(MDK_Object*);

struct MDK_Object_struct {
  MDK_TypeID id;
  
  unsigned refs;
  MDK_Object_Destructor destructor;
};

MDK_Object* MDK_Object_create(MDK_Object_Destructor destructor, size_t size);

void MDK_Object_ref(MDK_Object* this);
void MDK_Object_unref(MDK_Object* this);
