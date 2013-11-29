#include "user.h"
#include <stdbool.h>

bool isEmpty(user aUser){
  return aUser.id == EMPTY || aUser.id == TOMBSTONE;
}