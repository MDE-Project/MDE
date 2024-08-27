#include <stdio.h>
#include <stdlib.h>

#include <MDK/Object.h>
#include <MDK/Shorthand.h>

MDK_Object* MDK_Object_create(MDK_Object_Destructor destructor, size_t size) {
  MDK_Object* this = malloc(size == 0 ? sizeof(MDK_Object) : size);
  
  this->id = MDK_Object_typeID;
  this->refs = 0;
  this->destructor = destructor;
  
  #ifdef MDK_Debug
    printf("MDK_Object: Created object %p\n", this);
  #endif
  
  return this;
}

void MDK_Object_ref(MDK_Object* this) {
  ENSURE(MDK_Object);
  this->refs++;
}

void MDK_Object_unref(MDK_Object* this) {
  ENSURE(MDK_Object);
  
  if (this->refs == 0) {
    fputs("MDK_Object: Attempted to unref never ref'd object? Freeing anyway.\n", stderr);
    #ifdef MDK_Debug
      printf("MDK_Object: Destroying object %p\n", this);
    #endif
    
    if (this->destructor) {
      this->destructor(this);
    }
    free(this);
  } else if (this->refs == 1) {
    #ifdef MDK_Debug
      printf("MDK_Object: Destroying object %p\n", this);
    #endif
    
    this->refs--;
    if (this->destructor) {
      this->destructor(this);
    }
    free(this);
  } else {
    this->refs--;
  }
}
