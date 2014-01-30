#ifndef USER
#define USER

#define TOMBSTONE -2
#define EMPTY -1

typedef struct{
  int id;
  unsigned int age;
  char name[20];
} user;

bool isEmpty(user);
user createEmptyUser();

#endif