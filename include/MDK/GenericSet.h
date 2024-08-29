#pragma once

#include <MDK/Object.h>
#include <MDK/TypeID.h>

MDK_TypeID_create(MDK_GenericSet, 0xCFB9BB58);

typedef struct {
  MDK_Object inherited;
  MDK_TypeID id;
  
  unsigned storageSize;
  size_t itemSize;
  unsigned length;
  void* rawArray;
} MDK_GenericSet;

MDK_GenericSet* MDK_GenericSet_create(size_t itemSize, unsigned storageSize);
void MDK_GenericSet_init(MDK_GenericSet* this, size_t itemSize, unsigned storageSize);
void MDK_GenericSet_destroy(MDK_GenericSet* this);

void MDK_GenericSet_push(MDK_GenericSet* this, void* item);
void MDK_GenericSet_remove(MDK_GenericSet* this, unsigned i);

unsigned MDK_GenericSet_getLength(MDK_GenericSet* this);
void* MDK_GenericSet_getRawArray(MDK_GenericSet* this);
