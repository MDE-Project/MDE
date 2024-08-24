#include <stdlib.h>
#include <string.h>

#include <MDK/Array.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_Array* MDK_Array_create(unsigned storageSize) {
  MDK_Array* this = OBJ_CREATE(MDK_Array);
  MDK_Array_init(this, storageSize);
  return this;
}

void MDK_Array_init(MDK_Array* this, unsigned storageSize) {
  this->id = MDK_Array_typeID;
  this->storageSize = storageSize;
  this->length = 0;
  this->rawArray = malloc(storageSize*sizeof(MDK_Object*));
}

void MDK_Array_destroy(MDK_Array* this) {
  ENSURE(MDK_Array);
  
  for (unsigned i = 0; i < this->length; i++) {
    UNREF(this->rawArray[i]);
  }
  
  free(this->rawArray);
}

void MDK_Array_push(MDK_Array* this, MDK_Object* object) {
  ENSURE(MDK_Array);
  
  if (this->length >= this->storageSize) {
    this->storageSize *= 2;
    this->rawArray = reallocarray(this->rawArray, sizeof(MDK_Object*), this->storageSize);
  }
  
  this->rawArray[this->length] = object;
  REF(object);
  this->length++;
}

void MDK_Array_remove(MDK_Array* this, unsigned i) {
  ENSURE(MDK_Array);
  
  if (i < this->length) {
    UNREF(this->rawArray[i]);
    memmove(&this->rawArray[i], &this->rawArray[i+1], (this->length-1-i)*sizeof(MDK_Object*));
    this->length--;
  }
}

unsigned MDK_Array_getLength(MDK_Array* this) {
  ENSURE(MDK_Array);
  return this->length;
}

MDK_Object** MDK_Array_getRawArray(MDK_Array* this) {
  ENSURE(MDK_Array);
  return this->rawArray;
}
