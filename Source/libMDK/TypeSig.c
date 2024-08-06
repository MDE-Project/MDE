#include <stdio.h>
#include <stdlib.h>

#include <MDK/TypeSig.h>

void MDK_TypeSig_checkFailed(MDK_TypeSig actual, MDK_TypeSig expected) {
  fprintf(stderr, "Type signature check failed. Expected %.4s, got %.4s\n", (char*)&expected, (char*)&actual);
  abort();
}
