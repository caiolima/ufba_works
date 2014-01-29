#include "user.h"
#ifndef PAGE
#define PAGE

#define REGISTROS_POR_PAGINA 3

typedef struct {
  user data[REGISTROS_POR_PAGINA];
  int overflow_addr;
} page;

typedef struct {
  user data[2];
  int next;
} overflow_page;

#endif