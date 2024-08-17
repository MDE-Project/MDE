#include <stdlib.h>

#include <MDK/Object.h>
#include <MDK/Set.h>
#include <MDK/Shorthand.h>

MDK_Set* MDK_Set_create(unsigned storageSize) {
  MDK_Set* this = OBJ_CREATE(MDK_Set);
  MDK_Set_init(this, storageSize);
  return this;
}

void MDK_Set_init(MDK_Set* this, unsigned storageSize) {
  this->id = MDK_Set_typeID;
  this->storageSize = storageSize;
  this->length = 0;
  this->rawArray = malloc(storageSize*sizeof(MDK_Object*));
}

void MDK_Set_destroy(MDK_Set* this) {
  MDK_TypeID_ensure(this->id, MDK_Set_typeID);
  
  for (unsigned i = 0; i < this->length; i++) {
    UNREF(this->rawArray[i]);
  }
  
  free(this->rawArray);
}

void MDK_Set_push(MDK_Set* this, MDK_Object* object) {
  MDK_TypeID_ensure(this->id, MDK_Set_typeID);
  
  if (this->length >= this->storageSize) {
    this->storageSize *= 2;
    this->rawArray = reallocarray(this->rawArray, sizeof(MDK_Object*), this->storageSize);
  }
  
  this->rawArray[this->length] = object;
  REF(object);
  this->length++;
}

void MDK_Set_remove(MDK_Set* this, unsigned i) {
  MDK_TypeID_ensure(this->id, MDK_Set_typeID);
  
  UNREF(this->rawArray[i]);
  this->rawArray[i] = this->rawArray[this->length-1];
  this->length--;
}

unsigned MDK_Set_getLength(MDK_Set* this) {
  MDK_TypeID_ensure(this->id, MDK_Set_typeID);
  return this->length;
}

MDK_Object** MDK_Set_getRawArray(MDK_Set* this) {
  MDK_TypeID_ensure(this->id, MDK_Set_typeID);
  return this->rawArray;
}
