#pragma once

#include <MDK/Object.h>
#include <MDK/TypeSig.h>

MDK_TypeSig_create(MDK_GenericSet, 'G', 'S', 'e', 't');

typedef struct {
  MDK_Object inherited;
  MDK_TypeSig sig;
  
  unsigned storageSize;
  size_t itemSize;
  unsigned length;
  void* rawArray;
} MDK_GenericSet;

MDK_GenericSet* MDK_GenericSet_create(size_t itemSize, unsigned initialSize);
void MDK_GenericSet_init(MDK_GenericSet* this, size_t itemSize, unsigned initialSize);

void MDK_GenericSet_push(MDK_GenericSet* this, void* item);
void MDK_GenericSet_remove(MDK_GenericSet* this, unsigned i);

unsigned MDK_GenericSet_getLength(MDK_GenericSet* this);
void* MDK_GenericSet_getRawArray(MDK_GenericSet* this);
