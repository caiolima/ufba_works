#include <stdio.h>
#include <string.h>
#include "user.h"
#include "page.h"
#include "HashedFile.h"

int main() {
  
  char *fileName = "teste.data";
  HashedFile aFile(fileName);

  user aUser;
  aUser.id = 0;
  strcpy(aUser.name,"teste");
  aUser.age = 12;
  
  for(int i = 0; i < 24; i += 3){
    aUser.id = i;
    aFile.add(aUser);
  }

  user o_user = aFile.get(15);

  return 0;
}
