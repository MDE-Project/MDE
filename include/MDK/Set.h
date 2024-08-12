#pragma once

#include <MDK/Object.h>
#include <MDK/TypeSig.h>

MDK_TypeSig_create(MDK_Set, 'S', 'e', 't', ' ');

typedef struct {
  MDK_Object inherited;
  MDK_TypeSig sig;
  
  unsigned storageSize;
  unsigned length;
  MDK_Object** rawArray;
} MDK_Set;

MDK_Set* MDK_Set_create(unsigned storageSize);
void MDK_Set_init(MDK_Set* this, unsigned storageSize);
void MDK_Set_destroy(MDK_Set* this);

void MDK_Set_push(MDK_Set* this, MDK_Object* object);
void MDK_Set_remove(MDK_Set* this, unsigned i);

unsigned MDK_Set_getLength(MDK_Set* this);
MDK_Object** MDK_Set_getRawArray(MDK_Set* this);
