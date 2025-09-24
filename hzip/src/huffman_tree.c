#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "min_heap.h"
#include "huffman_tree.h"

/**
 * @brief 构建哈夫曼树
 * @param freq 文本字节频率表
 * @return NULL 频率表中内容全为 0
 * @return root 构造的哈夫曼树的根节点
 */
Node* build_huffman_tree(int freq[]) {
  MinHeap* heap = malloc(sizeof(MinHeap));
  if (!heap) {
    perror("malloc");
    return NULL;
  }

  heap_init(heap);

  for (int i = 0; i < 256; ++ i) {
    if (freq[i] > 0) {
      Node *node = create_node(i, freq[i]);
      heap_push(heap, node);
    }
  }

  // 空堆
  if (heap->size == 0) {
    free(heap);
    return NULL;
  }

  // 合并直到只剩一个节点
  Node *root;
  Node *x, *y;
  while (heap->size > 1) {
    x = heap_pop(heap);
    y = heap_pop(heap);

    root = create_node(-1, x->freq + y->freq);
    root->left = x;
    root->right = y;
    
    heap_push(heap, root);
  }

  root = heap_pop(heap);

  free(heap);

  return root;
}

/**
 * @brief 释放哈夫曼树各个节点的内存空间
 * @param root 哈夫曼树根节点
 */
void free_huffman_tree(Node *root) {
  // 后序遍历释放节点
  if (root == NULL) return;

  free_huffman_tree(root->left);
  free_huffman_tree(root->right);
  free(root);
}

/**
 * @brief 构建哈夫曼编码
 * @param root 哈夫曼树根节点
 * @param path 存储从根到叶子遍历过程中的01序列（编码）
 * @param depth 当前节点的深度（递归深度）
 * @param codes 存储哈夫曼编码表
 */
void build_huffman_codes(Node *root, char *path, int depth, char *codes[]) {
  if (root == NULL) return;

  // 叶子节点
  if (root->left == NULL && root->right == NULL) {
    path[depth] = '\0';

    // 特殊情况，只有一个唯一字节
    if (depth == 0) strcpy(path, "0");

    char *code = malloc((depth + 2) * sizeof(char));
    if (!code) {
      perror("malloc");
      return;
    }

    strcpy(code, path);
    codes[root->byte] = code;

    return;
  }

  // 往左走
  path[depth] = '0';
  build_huffman_codes(root->left, path, depth + 1, codes);

  // 往右走
  path[depth] = '1';
  build_huffman_codes(root->right, path, depth + 1, codes);
}

/**
 * @brief 释放哈夫曼编码表中的内容
 * @param codes 哈夫曼编码表
 */
void free_huffman_codes(char *codes[]) {
  if (!codes) return;
  
  for (int i = 0; i < 256; ++ i) {
    if (!codes[i]) free(codes[i]);
  }
}

/**
 * @brief 创建一个哈夫曼树节点
 * @param byte 字节值，用于初始化节点
 * @param freq 频率值，用于初始化节点
 * @return node 创建的节点指针
 */
Node* create_node(int byte, int freq) {
  Node *node = malloc(sizeof(Node));
  node->byte = byte;
  node->freq = freq;
  node->left = NULL;
  node->right = NULL;

  return node;
}
