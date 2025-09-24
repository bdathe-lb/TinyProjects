#ifndef __HUFFMAN_TREE_H__
#define __HUFFMAN_TREE_H__

#include "min_heap.h"

#define MAX_DEPTH 256

/// 哈夫曼树节点定义
typedef struct Node {
  int byte;
  int freq;
  struct Node *left;
  struct Node *right;
} Node;

/// 主要函数
Node* build_huffman_tree(int freq[]);
void free_huffman_tree(Node *root);
void build_huffman_codes(Node *root, char *path, int depth, char *codes[]);
void free_huffman_codes(char *codes[]);

/// 辅助函数
Node* create_node(int byte, int freq);

#endif  // __HUFFMAN_TREE_H__