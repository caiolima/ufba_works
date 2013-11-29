#include <stdio.h>
#include <stdlib.h>
#include "operation.h"

void destroy(operation *operationList){
  operation *current = operationList;
  
  do {
    operation *next = current->next;
    free(current);
    current = next;
  } while(current != NULL);
}