#pragma once

#include <stdint.h>

#define MDK_TypeID_create(type, c1, c2, c3, c4) \
  enum { \
    type##_typeID = ((c1) | (c2) << 8 | (c3) << 16 | (c4) << 24) \
  }

typedef uint32_t MDK_TypeID;

void MDK_TypeID_checkFailed(MDK_TypeID actual, MDK_TypeID expected);

static inline void MDK_TypeID_ensure(MDK_TypeID actual, MDK_TypeID expected) {
  if (actual != expected) {
    MDK_TypeID_checkFailed(actual, expected);
  }
}
