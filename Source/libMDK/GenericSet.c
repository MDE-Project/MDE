#include <stdlib.h>
#include <string.h>

#include <MDK/GenericSet.h>
#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_GenericSet* MDK_GenericSet_create(size_t itemSize, unsigned storageSize) {
  MDK_GenericSet* this = OBJ_CREATE(MDK_GenericSet);
  MDK_GenericSet_init(this, itemSize, storageSize);
  return this;
}

void MDK_GenericSet_init(MDK_GenericSet* this, size_t itemSize, unsigned storageSize) {
  this->id = MDK_GenericSet_typeID;
  this->storageSize = storageSize;
  this->itemSize = itemSize;
  this->length = 0;
  this->rawArray = malloc(storageSize*itemSize);
}

void MDK_GenericSet_destroy(MDK_GenericSet* this) {
  ENSURE(MDK_GenericSet);
  free(this->rawArray);
}

void MDK_GenericSet_push(MDK_GenericSet* this, void* item) {
  ENSURE(MDK_GenericSet);
  
  if (this->length >= this->storageSize) {
    this->storageSize *= 2;
    this->rawArray = reallocarray(this->rawArray, this->itemSize, this->storageSize);
  }
  
  memcpy(this->rawArray + this->length*this->itemSize, item, this->itemSize);
  this->length++;
}

void MDK_GenericSet_remove(MDK_GenericSet* this, unsigned i) {
  ENSURE(MDK_GenericSet);
  
  if (i < this->length) {
    memcpy(this->rawArray + i*this->itemSize, this->rawArray + (this->length-1)*this->itemSize, this->itemSize);
    this->length--;
  }
}

unsigned MDK_GenericSet_getLength(MDK_GenericSet* this) {
  ENSURE(MDK_GenericSet);
  return this->length;
}

void* MDK_GenericSet_getRawArray(MDK_GenericSet* this) {
  ENSURE(MDK_GenericSet);
  return this->rawArray;
}
