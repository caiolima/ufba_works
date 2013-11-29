#include "user.h"
#ifndef TOPERATION
#define TOPERATION

typedef struct tOperation{
  int from;
  int to;

  struct tOperation *next;
} operation;

void destroy(operation *);

#endif