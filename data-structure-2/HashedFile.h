#include "user.h"
#include "page.h"

#ifndef HASHEDFILE
#define HASHEDFILE

#define N 3

typedef struct{
  int level;
  int next;
} f_header;

int hash(int, int);

class HashedFile{

private:
  char *file_name;

public:
  f_header header;
  
  HashedFile(char *);
  void init();
  void add(user);
};

#endif