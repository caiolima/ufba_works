#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "node.h"
#include "HashedFile.h"
#include "user.h"
#include "operation.h"

bool isRoot(node *aNode){
  return aNode->parent == NULL;
}

bool isRightChild(node *aNode){
  if (isRoot(aNode)) {
    return false;
  }
  
  return aNode->parent->rChild == aNode;
}

bool isLeftChild(node *aNode){
  if (isRoot(aNode)) {
    return false;
  }
  
  return aNode->parent->lChild == aNode;
}

node* findChangePlace(node *aNode){
  
  if(isRoot(aNode))
    return NULL;

  if(isRightChild(aNode))
    return aNode->parent;

  node *lastParent = aNode->parent;
  while(!(isRoot(lastParent) || isRightChild(lastParent))){
    lastParent = lastParent->parent;
  }
  
  if(isRoot(lastParent))
    return NULL;

  return lastParent->parent;
}

int getNextPosition(int key, node* aNode){
  
  if(isRightChild(aNode))
    return (hash2(aNode->parent->key) + aNode->parent->pos) % FILE_SIZE;

  node *lastParent = aNode->parent;
  while(!(isRoot(lastParent) || isRightChild(lastParent))){
    lastParent = lastParent->parent;
  }

  if(isRoot(lastParent)){
    return (hash2(key) + aNode->parent->pos) % FILE_SIZE;
  }

  return (hash2(lastParent->parent->key) + aNode->parent->pos) % FILE_SIZE;

}

void createBTreeByLevel(int key, node **parentList, HashedFile f, int level) {
  int nChildren = 1 << level;
  node **children = (node **) malloc(sizeof(node *) * nChildren);

  for (int i = 1; i <= nChildren; i++){
    
    int iParent = ((i % 2) ? i / 2 + 1 : i / 2) - 1;
    int iChild = i - 1;

    children[iChild] = (node *) malloc(sizeof(node));
    children[iChild]->parent = parentList[iParent];
    if (i % 2) { //left child
      parentList[iParent]->lChild = children[iChild];
    } else { //right child
      parentList[iParent]->rChild = children[iChild];
    }
     
    int pos = getNextPosition(key, children[iChild]);

    children[iChild]->pos = pos;
    children[iChild]->rChild = NULL;
    children[iChild]->lChild = NULL;
    
    user user = f.getInPosition(pos);
    if(isEmpty(user)){
      children[iChild]->key = EMPTY;
      
      return ;
    }

    children[iChild]->key = user.id;
  }

  //There is no space in this level, calculate next
  createBTreeByLevel(key, children, f, level + 1); 
  free(children);
}

node* createBTree(int key, HashedFile f){
  
  node *aNode = (node *) malloc(sizeof(node));
  int pos = hash1(key);
  user aUser = f.getInPosition(pos); 
  
  aNode->pos = pos;
  aNode->rChild = NULL;
  aNode->lChild = NULL;
  aNode->parent = NULL;

  if(isEmpty(aUser)){
    aNode->key = EMPTY;

    return aNode;
  }

  aNode->key = aUser.id;

  createBTreeByLevel(key, &aNode, f, 1);

  return aNode;
}

void destroy(node *aNode){
  if (aNode == NULL) {
    return;
  }
  destroy(aNode->rChild);
  destroy(aNode->lChild);

  free(aNode);
}

void addLeftChild(node *parent, node *child){
  parent->lChild = child;
  child->parent = parent;
}

void addRightChild(node *parent, node *child){
  parent->rChild = child;
  child->parent = parent;
}

node* createTestBTree(int l, int parent, int side){

  node *aNode = (node *) malloc(sizeof(node));
  
  if(side == -1)
    aNode->pos = 0;
  else
    aNode->pos = parent * 2 + side;
  
  aNode->rChild = NULL;
  aNode->lChild = NULL;
  
  if(l > 0){
    addLeftChild(aNode, createTestBTree(l-1, aNode->pos, 1));
    addRightChild(aNode, createTestBTree(l-1, aNode->pos, 2));
  }

  
  return aNode;
}

void printTrySeqLevel(node **nodeList, int level) {
  int nNodes = 1 << level;
  int nChildren = nNodes << 1;
  node **children = (node **) malloc(sizeof(node *) * nChildren);

  int iChildren = 0;
  for (int i = 0; i < nNodes; i++) {
    if(nodeList[i]->lChild != NULL){
      children[iChildren] = nodeList[i]->lChild;
      iChildren++;
    }
    
    if(nodeList[i]->rChild != NULL){
      children[iChildren] = nodeList[i]->rChild;
      iChildren++;
    }

    if (nodeList[i]->key == EMPTY) {
      printf("%d: %s\n", nodeList[i]->pos, "vazio");
      free(children);
      return;
    } else {
      printf("%d: %d\n", nodeList[i]->pos, nodeList[i]->key);
    }
  }

  if (iChildren > 0) {
    printTrySeqLevel(children, level+1);
  }
  free(children);
}

void printTrySeq(node* node) {
  printTrySeqLevel(&node, 0);
}

node *findEmptySpace(node *aNode){
  if (aNode->key == EMPTY)
    return aNode;

  if(aNode->lChild != NULL){
    node *lChild = findEmptySpace(aNode->lChild);
    if(lChild->key == EMPTY)
      return lChild;
  }
  if(aNode->rChild != NULL){
    node *rChild = findEmptySpace(aNode->rChild);
    if(rChild->key == EMPTY)
      return rChild;
  }

  return aNode;
}

operation* getOperationList(node *aNode){
  node *moveTo = findEmptySpace(aNode);
  node *moveFrom = findChangePlace(moveTo);
  operation *lastOperation = NULL;
  operation *firstOperation = NULL;

  while(moveFrom != NULL){
    operation *o = (operation *) malloc(sizeof(operation));

    o->from = moveFrom->pos;
    o->to = moveTo->pos;

    if (lastOperation != NULL) {
      lastOperation->next = o;
    } else {
      firstOperation = o;
    }

    lastOperation = o;
    // printf("%d -> %d \n", moveFrom->pos, moveTo->pos);
    moveTo = moveFrom;
    moveFrom = findChangePlace(moveFrom);
  }

  operation *o = (operation *) malloc(sizeof(operation));
  o->from = -1;
  o->to = moveTo->pos;
  o->next = NULL;
  if(firstOperation == NULL) {
    firstOperation = o;
  } else {
    lastOperation->next = o;
  }

  return firstOperation;
  // printf("new -> %d \n", moveTo->pos);
}