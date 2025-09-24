#ifndef __MIN_HEAP_H__
#define __MIN_HEAP_H__

#include <stdbool.h>

#define MAX_NODE_NUM 512

#define PARENT_IDX(i) ((i) / 2)
#define LCHILD_IDX(i) (2 * (i))
#define RCHILD_IDX(i) (2 * (i) + 1)

typedef struct Node Node;

/// 小根堆定义
typedef struct {
  Node *data[MAX_NODE_NUM];
  int size;
} MinHeap;

/// 主要函数
void heap_init(MinHeap *h);
void heap_push(MinHeap *h, Node *node);
Node* heap_pop(MinHeap *h);

/// 辅助函数
bool heap_is_empty(MinHeap *h);
bool node_is_leaf(Node *node);
void swap(Node **a, Node **b);

#endif  // __MIN_HEAP_H__