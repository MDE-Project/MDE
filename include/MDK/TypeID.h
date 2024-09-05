#pragma once

#include <stdint.h>

#define MDK_TypeID_create(TYPE, VALUE) \
  enum { \
    TYPE##_typeID = (VALUE) \
  }

typedef uint32_t MDK_TypeID;

void MDK_TypeID_checkFailed();

static inline void MDK_TypeID_ensure(MDK_TypeID actual, MDK_TypeID expected) {
  if (actual != expected) {
    MDK_TypeID_checkFailed();
  }
}
