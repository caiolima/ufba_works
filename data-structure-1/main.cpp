#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "user.h"
#include "HashedFile.h"
#include "node.h"

//print user infos
//-------------------------------------------------------
void printUser(user aUser) {
  printf("id: %d\nnome: %s\nidade: %d\n", aUser.id, aUser.name, aUser.age);
}

//Main
//-----------------------------------------------------------
void insertOperation(HashedFile file) {

  unsigned int id;
  scanf("%d", &id);

  char name[20];
  scanf("%s", name);

  unsigned int age;
  scanf("%d", &age);

  user aUser = file.get(id);
  
  if (!isEmpty(aUser)){ //Is a valid key
    printf("chave ja existente: %d\n", id);

    return;
  }

  aUser.id = id;
  strcpy(aUser.name, name);
  aUser.age = age;

  file.write(id, aUser);

}

void searchOperation(HashedFile f) {
  unsigned int id;
  scanf("%d", &id);

  user aUser = f.get(id);
  if (isEmpty(aUser)) {
    printf("chave nao encontrada: %d\n", id);
  } else {
    printf("chave: %d\n%s\n%d\n",
            aUser.id,
            aUser.name,
            aUser.age);
  }
}

void removeOperation(HashedFile f){
  unsigned int id;
  scanf("%d", &id);

  if(!f.remove(id)){
    printf("chave nao encontrada: %d\n", id);
  }
}

void printOperation(HashedFile f) {
  for (int i = 0; i < FILE_SIZE; i++) {
    user aUser = f.getInPosition(i);
    if (aUser.id == EMPTY){
      printf("%d: vazio\n", i);
    } else if(aUser.id == TOMBSTONE){
      printf("%d: *\n", i);
    } else {
      printf("%d: %d %s %d\n", i, aUser.id, aUser.name, aUser.age);
    }
  }
}

void treeOperation(HashedFile f){
  unsigned int key;
  scanf("%d", &key);

  node *root = createBTree(key, f);  

  printTrySeq(root);

  destroy(root); 
}

void executeOperation(char op, HashedFile file) {
  switch (op) {
  case 'i':
    insertOperation(file);
    break;
  case 'c':
    searchOperation(file);
    break;
  case 'r':
    removeOperation(file);
    break;
  case 'p':
    printOperation(file);
    break;
  case 't':
    treeOperation(file);
    break;
  case 'e':
    break;
  default:;
  }
}

void insertTestData(int id, int pos, HashedFile f){
  //test records
  user u;
  u.id = id;
  strcpy(u.name, "teste");
  u.age = 13;

  f.writeInPosition(pos, u);
}

void testData(HashedFile f){
  
  insertTestData(41, 0, f);
  insertTestData(39, 1, f);
  insertTestData(13, 2, f);
  insertTestData(27, 5, f);
  insertTestData(16, 6, f);
  insertTestData(18, 7, f);
  insertTestData(17, 8, f);
  insertTestData(29, 9, f);
  insertTestData(28, 10, f);

}

int main() {
  char *fileName = "teste.data";
  HashedFile aFile(fileName);

  testData(aFile);

  char op;
  do {
    scanf("%c", &op);
    executeOperation(op, aFile);
  } while (op != 'e');
  

  return 0;
}