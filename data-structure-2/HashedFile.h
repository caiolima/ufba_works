#include <list>
#include "user.h"
#include "page.h"

#ifndef HASHEDFILE
#define HASHEDFILE

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
  void reorganize(int);
  int numberOfAccess(int);

public:
  f_header header;
  
  HashedFile(char *);
  void init();
  void add(user);
  void addAndReorganize(user);
  bool removeAndReorganize(int key);
  bool remove(int);
  user get(int);
  int getPageHash(int);
  page readPage(int);
  void persistPage(page, int);
  void persistHeader();
  float packingFactor();
  std::list<user> getAllDataFromPage(int);
  void split();
  void unsplit();
  float avaregeNumberOfAccess();

};

#endif