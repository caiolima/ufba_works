#include <list>
#include "user.h"
#include "page.h"

#ifndef HASHEDFILE
#define HASHEDFILE

#define N 3

typedef struct{
  int level;
  int next;
  int used_space;
  int total_space;
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
  bool remove(int);
  user get(int);
  int getPageHash(int);
  page readPage(int);
  void persistPage(page, int);
  void persistHeader();
  float spaceUtilization();
  std::list<user> getAllDataFromPage(int);

};

#endif