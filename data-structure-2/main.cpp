#include <stdio.h>
#include <string.h>
#include <list>
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

  std::list<user> user_list = aFile.getAllDataFromPage(0);
  for (std::list<user>::iterator it=user_list.begin(); it != user_list.end(); ++it) {
    printf("%d\n", it->id);
  }
  return 0;
}
