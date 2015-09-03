#include <stdio.h>
#include <stdlib.h>

void check(int res, const char *msg) {
  if(!res) {
    perror(msg);
    exit(1);
  }
}

struct Node {
  int value, key, nearMax;
};

void *allocateNodes(int size) {
  struct Node *nodesP = malloc(sizeof(struct Node)*size);
  check(nodesP != NULL, "can't allocate memory for nodes");
  return nodesP;
}

typedef struct {
  struct Node *contents;
  size_t top, maxSize;
} stackT;

void *allocateStack(int maxSize) {
  stackT *stackP = malloc(sizeof(stackT));
  check(stackP != NULL, "can't allocate memory");

  stackP->contents = malloc(sizeof(struct Node)*maxSize);
  check(stackP->contents != NULL, "can't allocate memory for stack");

  stackP->maxSize = maxSize;
  stackP->top = 0;

  return stackP;
}

int stackIsEmpty(stackT *stackP) {
  return stackP->top == 0;
}

int stackIsFull(stackT *stackP) {
  return stackP->top >= stackP->maxSize;
}

struct Node stackTop(stackT *stackP) {
  return stackP->contents[stackP->top-1];
}

void stackPush(stackT *stackP, struct Node element) {
  check(!stackIsFull(stackP), "can't push: stack is full");
  stackP->contents[stackP->top++] = element;
}

struct Node stackPop(stackT *stackP) {
  check(!stackIsEmpty(stackP), "can't pop: stack is empty");
  return stackP->contents[--stackP->top];
}

void freeStack(stackT* stackP) {
  free(stackP->contents);
  free(stackP);
}

#define TRAVERSE_INORDER 0
#define TRAVERSE_POSTORDER 1

void traverseTree(struct Node *nodes, size_t left, size_t right, int traverseType) {
  if (left > right) {
      return;
  }
  if (left == right) {
    printf("%d ", nodes[left].value);
    return;
  }
  traverseTree(nodes, left+1, nodes[left].nearMax-1, traverseType);

  if(TRAVERSE_INORDER == traverseType) {
      printf("%d ", nodes[left].value);
  }
  traverseTree(nodes, nodes[left].nearMax, right, traverseType);

  if(TRAVERSE_POSTORDER == traverseType) {
      printf("%d ", nodes[left].value);
  }
}

int main() {
  int n;
  check(scanf("%d", &n) == 1, "can't read value");

  struct Node *nodesP = allocateNodes(n);

  for(size_t i = 0; i < n; i++) {
    check(scanf("%d", &nodesP[i].value) == 1, "can't read value");
    nodesP[i].key = i;
  }

  stackT *stackP = allocateStack(n);

  for(size_t i = n; i > 0; i--) {
    while(!stackIsEmpty(stackP) && stackTop(stackP).value < nodesP[i-1].value) {
      stackPop(stackP);
    }
    if(stackIsEmpty(stackP)) {
      nodesP[i-1].nearMax = n;
    } else {
      nodesP[i-1].nearMax = stackTop(stackP).key;
    }
    stackPush(stackP, nodesP[i-1]);
  }
  freeStack(stackP);

  traverseTree(nodesP, 0, n-1, TRAVERSE_POSTORDER);
  printf("\n");
  traverseTree(nodesP, 0, n-1, TRAVERSE_INORDER);
  printf("\n");

  free(nodesP);

  return 0;
}
