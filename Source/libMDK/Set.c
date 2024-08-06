#include <stdlib.h>

#include <MDK/Object.h>
#include <MDK/Set.h>
#include <MDK/Shorthand.h>

MDK_Set* MDK_Set_create(unsigned initialSize) {
  MDK_Set* this = OBJ_CREATE(MDK_Set);
  MDK_Set_init(this, initialSize);
  return this;
}

void MDK_Set_init(MDK_Set* this, unsigned initialSize) {
  this->sig = MDK_Set_typeSig;
  this->storageSize = initialSize;
  this->length = 0;
  this->rawArray = malloc(initialSize*sizeof(MDK_Object*));
}

void MDK_Set_destroy(MDK_Set* this) {
  MDK_TypeSig_ensure(this->sig, MDK_Set_typeSig);
  
  for (unsigned i = 0; i < this->length; i++) {
    UNREF(this->rawArray[i]);
  }
  
  free(this->rawArray);
}

void MDK_Set_push(MDK_Set* this, MDK_Object* object) {
  MDK_TypeSig_ensure(this->sig, MDK_Set_typeSig);
  
  if (this->length >= this->storageSize) {
    this->storageSize *= 2;
    this->rawArray = reallocarray(this->rawArray, sizeof(MDK_Object*), this->storageSize);
  }
  
  this->rawArray[this->length] = object;
  REF(object);
  this->length++;
}

void MDK_Set_remove(MDK_Set* this, unsigned i) {
  MDK_TypeSig_ensure(this->sig, MDK_Set_typeSig);
  
  UNREF(this->rawArray[i]);
  this->rawArray[i] = this->rawArray[this->length-1];
  this->length--;
}

unsigned MDK_Set_getLength(MDK_Set* this) {
  MDK_TypeSig_ensure(this->sig, MDK_Set_typeSig);
  return this->length;
}

MDK_Object** MDK_Set_getRawArray(MDK_Set* this) {
  MDK_TypeSig_ensure(this->sig, MDK_Set_typeSig);
  return this->rawArray;
}
