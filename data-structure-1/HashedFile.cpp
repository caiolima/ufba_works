#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "user.h"
#include "HashedFile.h"
#include "operation.h"
#include "node.h"

//Hash funcitons
//--------------------------------------------------------
int hash1(int key) {
  return key % FILE_SIZE;
}

int hash2(int key) {
  int result = key/FILE_SIZE;
  if(result == 0)
    result = 1;
  return result;
}

//File operations definitions
//------------------------------------------------------------------
HashedFile::HashedFile(char* filename) {
  this->file_name = filename;
  this->init();
}

void HashedFile::init(){
  FILE *f = fopen(this->file_name, "r");
  if(f){
    //File exists
    fclose(f);
    return;
  }

  f = fopen(this->file_name, "w");
  user users[FILE_SIZE];

  for (int i = 0; i < FILE_SIZE; i++) {
    users[i].id = EMPTY;
    strcpy(users[i].name, "");
    users[i].age = 0;
  }

  fwrite(users, sizeof(user), FILE_SIZE, f);

  fclose(f);

}

int HashedFile::getPos(int key){
  return hash1(key);
}

bool HashedFile::writeInPosition(int pos, user data){
  FILE *f = fopen(this->file_name, "r+");

  fseek(f, sizeof(user)*pos, SEEK_SET);

  fwrite(&data, sizeof(user), 1, f);

  fclose(f);

  return true;
}

bool HashedFile::write(int key, user data) {
   node *root = createBTree(key, *this);
   operation* seqOperation = getOperationList(root);

   this->execute(seqOperation, data);
   destroy(root); 
}

bool HashedFile::remove(int key) {
  user aUser = this->get(key);
  if (isEmpty(aUser)) {
    return false;
  }

  int pos = this->getPos(key);

  FILE *f = fopen(this->file_name, "r+");

  fseek(f, sizeof(user)*pos, SEEK_SET);
  
  user tombuser;
  tombuser.id = TOMBSTONE;
  strcpy(tombuser.name, "");
  tombuser.age = 0;
  
  fwrite(&tombuser, sizeof(user), 1, f);

  fclose(f);

  return true;
}

user HashedFile::get(int key) {
  user aUser;

  int pos = this->getPos(key);

  return this->getInPosition(pos);
    
}

user HashedFile::getInPosition(int pos){
  user aUser;

  FILE *f = fopen(this->file_name, "r");

  fseek(f, sizeof(user)*pos, SEEK_SET);

  fread(&aUser, sizeof(user), 1, f);

  fclose(f);
  return aUser;
}

void HashedFile::execute(operation *opList, user newData) { 
  operation *currOp  = opList;
  do{
    if(currOp->from == -1){
      this->writeInPosition(currOp->to, newData);
    } else {
      user aUser = this->getInPosition(currOp->from);
      this->writeInPosition(currOp->to, aUser);
    }
    
    currOp = currOp->next;
  }while(currOp != NULL);
  destroy(opList);
}
