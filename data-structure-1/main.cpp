#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "user.h"
#include "File.h"

#define FILE_SIZE 11

//File operations definitions
File::File(char* filename){
  this->file_name = filename;
  this->init();
}

void File::init(){
  FILE *f = fopen(this->file_name, "r");
  if(f){
    //File exists
    fclose(f);
    return;
  }

  f = fopen(this->file_name, "w");
  user users[FILE_SIZE];

  for(int i = 0; i < FILE_SIZE; i++){
    users[i].id = -1;
    strcpy(users[i].name, "");
    users[i].age = 0;
  }

  fwrite(users, sizeof(user), FILE_SIZE, f);

  fclose(f);

}

bool File::write(int pos, user data){
  FILE *f = fopen(this->file_name, "r+");

  fseek(f, sizeof(user)*pos, SEEK_SET);

  fwrite(&data, sizeof(user),1, f);

  fclose(f);

  return true;
}

user File::get(int pos){
  user a;
  return a;
}

int main(){
  
  user aUser;
  aUser.id = 3;
  strcpy(aUser.name, "Caio");
  aUser.age = 20;

  char *fileName = "teste.data";
  File aFile(fileName);

  aFile.write(4, aUser);

  return 0;

}