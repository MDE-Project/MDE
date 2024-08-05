#include <stdio.h>
#include <stdlib.h>

#include <MDK/Object.h>

MDK_Object* MDK_Object_create(MDK_Object_Destructor destructor, size_t size) {
  MDK_Object* this = malloc(size == 0 ? sizeof(MDK_Object) : size);
  
  this->sig = MDK_Object_typeSig;
  this->refs = 0;
  this->destructor = destructor;
  
  #ifdef MDK_Debug
    printf("Created object %p\n", this);
  #endif
  
  return this;
}

void MDK_Object_ref(MDK_Object* this) {
  MDK_TypeSig_ensure(this->sig, MDK_Object_typeSig);
  this->refs++;
}

void MDK_Object_unref(MDK_Object* this) {
  MDK_TypeSig_ensure(this->sig, MDK_Object_typeSig);
  
  if (this->refs == 0) {
    fputs("Attempted to unref never ref'd object? Freeing anyway.\n", stderr);
    #ifdef MDK_Debug
      printf("Destroying object %p\n", this);
    #endif
    
    if (this->destructor) {
      this->destructor(this);
    }
    free(this);
  } else if (this->refs == 1) {
    #ifdef MDK_Debug
      printf("Destroying object %p\n", this);
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
