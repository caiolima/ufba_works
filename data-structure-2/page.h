#include <stdbool.h>
#include "user.h"

#ifndef PAGE
#define PAGE

#define N 2
#define REGISTROS_POR_PAGINA 3
#define OVERFLOW_N 3

typedef struct {
  user data[REGISTROS_POR_PAGINA];
  int overflow_addr;
} page;

typedef struct {
  user data[OVERFLOW_N];
  int next;
  bool avaiable;
} overflow_page;

overflow_page createEmptyOverflowPage();
overflow_page getOverflowPage(int);
void persistOverflowPage(int, overflow_page);
int allocOverflowPage();
bool insertOn(overflow_page*, user);
bool deleteRecordOn(overflow_page*, int);
bool isOverflowPageEmpty(overflow_page);

#endif
