#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_Array, 0x405EB567);

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  unsigned storageSize;
  unsigned length;
  MDK_Object** rawArray;
} MDK_Array;

MDK_Array* MDK_Array_create(unsigned storageSize);
void MDK_Array_init(MDK_Array* this, unsigned storageSize);
void MDK_Array_destroy(MDK_Array* this);

void MDK_Array_push(MDK_Array* this, MDK_Object* object);
void MDK_Array_remove(MDK_Array* this, unsigned i);

unsigned MDK_Array_getLength(MDK_Array* this);
MDK_Object** MDK_Array_getRawArray(MDK_Array* this);
