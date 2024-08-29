#include <stdio.h>
#include <stdlib.h>

#include <MDK/TypeID.h>

void MDK_TypeID_checkFailed() {
  fputs("MDK_TypeID: Type signature check failed\n", stderr);
  abort();
}
