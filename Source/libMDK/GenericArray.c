#include <stdlib.h>
#include <string.h>

#include <MDK/GenericArray.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_GenericArray* MDK_GenericArray_create(size_t itemSize, unsigned storageSize) {
  MDK_GenericArray* this = OBJ_CREATE(MDK_GenericArray);
  MDK_GenericArray_init(this, itemSize, storageSize);
  return this;
}

void MDK_GenericArray_init(MDK_GenericArray* this, size_t itemSize, unsigned storageSize) {
  this->id = MDK_GenericArray_typeID;
  this->storageSize = storageSize;
  this->itemSize = itemSize;
  this->length = 0;
  this->rawArray = malloc(storageSize*itemSize);
}

void MDK_GenericArray_destroy(MDK_GenericArray* this) {
  ENSURE(MDK_GenericArray);
  free(this->rawArray);
}

void MDK_GenericArray_push(MDK_GenericArray* this, void* item) {
  ENSURE(MDK_GenericArray);
  
  if (this->length >= this->storageSize) {
    this->storageSize *= 2;
    this->rawArray = reallocarray(this->rawArray, this->itemSize, this->storageSize);
  }
  
  memcpy(this->rawArray + this->length*this->itemSize, item, this->itemSize);
  this->length++;
}

void MDK_GenericArray_remove(MDK_GenericArray* this, unsigned i) {
  ENSURE(MDK_GenericArray);
  
  if (i < this->length) {
    memmove(this->rawArray + i*this->itemSize, this->rawArray + (i+1)*this->itemSize, (this->length-1-i)*this->itemSize);
    this->length--;
  }
}

unsigned MDK_GenericArray_getLength(MDK_GenericArray* this) {
  ENSURE(MDK_GenericArray);
  return this->length;
}

void* MDK_GenericArray_getRawArray(MDK_GenericArray* this) {
  ENSURE(MDK_GenericArray);
  return this->rawArray;
}
