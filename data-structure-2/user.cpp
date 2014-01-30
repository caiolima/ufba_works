#include <string.h>
#include "user.h"
#include <stdbool.h>

bool isEmpty(user aUser){
  return aUser.id == EMPTY || aUser.id == TOMBSTONE;
}

user createEmptyUser() {
  user aUser;

  aUser.id = EMPTY;
  strcpy(aUser.name, "");
  aUser.age = 0;

  return aUser;
}
