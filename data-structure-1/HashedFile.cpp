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
  return key % TAMANHO_ARQUIVO;
}

int hash2(int key) {
  int result = key/TAMANHO_ARQUIVO;
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
  user users[TAMANHO_ARQUIVO];

  for (int i = 0; i < TAMANHO_ARQUIVO; i++) {
    users[i].id = EMPTY;
    strcpy(users[i].name, "");
    users[i].age = 0;
  }

  fwrite(users, sizeof(user), TAMANHO_ARQUIVO, f);

  fclose(f);

}

int HashedFile::getNumberOfAccess(int key) {
  int homeAdress = hash1(key);
  int inc = hash2(key);
  int currentPos = homeAdress;

  int count = 0;
  do {
    user aUser = this->getInPosition(currentPos);
    count++;
    if (aUser.id == key){
      return count;
    }

    if (aUser.id == EMPTY) {
      return -1; 
    }
      
    currentPos += inc;
    currentPos = currentPos % TAMANHO_ARQUIVO;
  } while (currentPos != homeAdress);

  return -1; //error
}

int HashedFile::getPos(int key){
  int homeAdress = hash1(key);
  int inc = hash2(key);
  int currentPos = homeAdress;

  do {
    user aUser = this->getInPosition(currentPos);
    if (aUser.id == key) {
      return currentPos;
    }

    if (aUser.id == EMPTY) {
      return -1;
    } 

    currentPos += inc;
    currentPos = currentPos % TAMANHO_ARQUIVO;
  } while (currentPos != homeAdress);

  return -1;
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
  int pos = this->getPos(key);

  //Didn't find
  user aUser;
  if(pos == -1){
    aUser.id = EMPTY;
  } else {
    aUser = this->getInPosition(pos);
  }

  return aUser;
    
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
