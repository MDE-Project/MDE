#pragma once

#include <stdint.h>

#define MDK_TypeSig_create(type, c1, c2, c3, c4) \
  enum { \
    type##_typeSig = ((c1) | (c2) << 8 | (c3) << 16 | (c4) << 24) \
  }

typedef uint32_t MDK_TypeSig;

void MDK_TypeSig_checkFailed(MDK_TypeSig actual, MDK_TypeSig expected);

static inline void MDK_TypeSig_ensure(MDK_TypeSig actual, MDK_TypeSig expected) {
  if (actual != expected) {
    MDK_TypeSig_checkFailed(actual, expected);
  }
}
