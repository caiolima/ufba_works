#include "HashedFile.h"
#include "operation.h"

#ifndef TNODE
#define TNODE

typedef struct tNode{
  struct tNode *parent;
  struct tNode *lChild;
  struct tNode *rChild;

  int pos;
  int key;
} node;

bool isRoot(node *);
bool isRightChild(node *);
bool isLeftChild(node *);

/* Returns the first parent that is a right child. If there is no, 
*  returns the root
*/
node* getParentChaining(node *);

node* createBTree(int, HashedFile);

node* createTestBTree(int, int, int);

void printTrySeq(node *); 

operation* getOperationList(node *);

void addLeftChild(node *, node *);
void addRightChild(node *, node *);

void destroy(node *);



#endif