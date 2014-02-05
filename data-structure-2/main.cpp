#include <stdio.h>
#include <string.h>
#include <list>
#include "user.h"
#include "page.h"
#include "HashedFile.h"

void printUser(user aUser) {
  printf("chave: %d nome: %s idade: %d\n", aUser.id, aUser.name, aUser.age);
}

void insertOperation(HashedFile *file) {

  unsigned int id;
  scanf("%d", &id);

  char name[20];
  scanf("%s", name);

  unsigned int age;
  scanf("%d", &age);

  user aUser = file->get(id);
  
  if (!isEmpty(aUser)){ //Is a valid key
    printf("chave ja existente: %d\n", id);

    return;
  }

  aUser.id = id;
  strcpy(aUser.name, name);
  aUser.age = age;

  file->addAndReorganize(aUser);
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

void removeOperation(HashedFile *f){
  unsigned int id;
  scanf("%d", &id);

  if(!f->removeAndReorganize(id)){
    printf("chave nao encontrada: %d\n", id);
  }
}

void printOperation(HashedFile f){
  printf("nivel: %d\n", f.header.level);
  printf("next: %d\n", f.header.next);

  int qtd_pages = f.header.next + N * (1 << f.header.level);
  for (int i = 0; i < qtd_pages; i++) {
    printf("pagina: %d\n", i);
    std::list<user> user_list = f.getAllDataFromPage(i);
    int cont_overflow_page = -1 * REGISTROS_POR_PAGINA;
    for (std::list<user>::iterator it=user_list.begin(); it != user_list.end(); ++it) {
      
      if(cont_overflow_page >= 0 && cont_overflow_page % OVERFLOW_N == 0) 
        printf("----------\n");
      
      printUser(*it);
      cont_overflow_page++;
    } 
  }
}

void avaregeAccessOperation(HashedFile file) {
  printf("%.1f\n", file.avaregeNumberOfAccess());
}

void executeOperation(char op, HashedFile *file) {
  switch (op) {
  case 'i':
    insertOperation(file);
    break;
  case 'c':
    searchOperation(*file);
    break;
  case 'r':
    removeOperation(file);
    break;
  case 'p':
    printOperation(*file);
    break;
  case 'm':
    avaregeAccessOperation(*file);
    break;
  case 'e':
    break;
  default:;
  }
}

int main() {
  
  char *fileName = "teste.data";
  HashedFile aFile(fileName);

  char op;
  do {
    scanf("%c", &op);
    executeOperation(op, &aFile);
  } while (op != 'e');

  return 0;
}
