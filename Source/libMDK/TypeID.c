#include <stdio.h>
#include <stdlib.h>

#include <MDK/TypeID.h>

void MDK_TypeID_checkFailed(MDK_TypeID actual, MDK_TypeID expected) {
  fprintf(stderr, "Type signature check failed. Expected %.4s, got %.4s\n", (char*)&expected, (char*)&actual);
  abort();
}
