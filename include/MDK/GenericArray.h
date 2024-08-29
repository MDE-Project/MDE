#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_GenericArray, 0x90717030);

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  unsigned storageSize;
  size_t itemSize;
  unsigned length;
  void* rawArray;
} MDK_GenericArray;

MDK_GenericArray* MDK_GenericArray_create(size_t itemSize, unsigned storageSize);
void MDK_GenericArray_init(MDK_GenericArray* this, size_t itemSize, unsigned storageSize);
void MDK_GenericArray_destroy(MDK_GenericArray* this);

void MDK_GenericArray_push(MDK_GenericArray* this, void* item);
void MDK_GenericArray_remove(MDK_GenericArray* this, unsigned i);

unsigned MDK_GenericArray_getLength(MDK_GenericArray* this);
void* MDK_GenericArray_getRawArray(MDK_GenericArray* this);
