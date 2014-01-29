#include <stdio.h>
#include <string.h>
#include "user.h"
#include "page.h"
#include "HashedFile.h"

int main() {
  
  char *fileName = "teste.data";
  HashedFile aFile(fileName);

  printf("%d\n%d\n", aFile.header.level, aFile.header.next);

  return 0;
}
