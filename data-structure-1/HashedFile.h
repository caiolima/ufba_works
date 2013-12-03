#include "user.h"
#include "operation.h"

#ifndef HASHEDFILE
#define HASHEDFILE

#define TAMANHO_ARQUIVO 11

int hash1(int);
int hash2(int);

class HashedFile{

private:
  char *file_name;
  void execute(operation *, user);

public:
  HashedFile(char *);
  void init();
  bool writeInPosition(int, user);
  bool write(int, user);
  bool remove(int);
  user get(int);
  user getInPosition(int);
  int getPos(int);
  int getNumberOfAccess(int);

};

#endif