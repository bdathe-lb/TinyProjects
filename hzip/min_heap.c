#include "min_heap.h"
#include "huffman_tree.h"
#include <stddef.h>

/**
 * @brief 初始化小根堆
 * @param h 小根堆
 */
void heap_init(MinHeap *h) {
  h->size = 0;
}

/**
 * @brief 插入新元素到小根堆
 * @param h 小根堆
 * @param node 待插入的新节点
 */
void heap_push(MinHeap *h, Node *node) {
  // 将新节点放到数组尾端
  h->data[++ h->size] = node;
  // 比较父节点，进行上浮操作
  int cur_idx = h->size;
  int parent_idx = PARENT_IDX(cur_idx);
  while (cur_idx != 1 && h->data[cur_idx]->freq < h->data[parent_idx]->freq) {
    swap(&h->data[cur_idx], &h->data[parent_idx]);
    cur_idx = parent_idx;
    parent_idx = PARENT_IDX(cur_idx);
  }

  return;
}

/**
 * @brief 取出堆顶元素，并调整小根堆
 * @param h 小根堆
 * @return NULL 小根堆为空
 * @return res 堆顶元素
 */
Node* heap_pop(MinHeap *h) {
  if (heap_is_empty(h)) return NULL;

  // 记录堆顶元素
  Node *res = h->data[1]; 
  // 将数组尾端元素移到数组首端
  h->data[1] = h->data[h->size --];
  // 比较左右孩子节点，进行下浮操作
  int cur_idx = 1;
  while (LCHILD_IDX(cur_idx) <= h->size) {  // 至少存在左孩子
    int lchild_idx = LCHILD_IDX(cur_idx);
    int rchild_idx = RCHILD_IDX(cur_idx);
    int smallest = cur_idx;

    if (h->data[lchild_idx]->freq < h->data[smallest]->freq) {
      smallest = lchild_idx;
    }

    if (rchild_idx <= h->size &&    
        h->data[rchild_idx]->freq < h->data[smallest]->freq) {  // 存在右孩子
      smallest = rchild_idx;
    }

    if (smallest == cur_idx) break;

    swap(&h->data[cur_idx], &h->data[smallest]);
    cur_idx = smallest;
  }

  return res;
}

/**
 * @brief 判断小根堆是否为空
 * @param h 小根堆
 * @return true 小根堆为空
 * @return false 小根堆不为空
 */
bool heap_is_empty(MinHeap *h) {
  return h->size == 0 ? true : false;
}

/**
 * @brief 判断节点是否是叶子节点
 * @param node 待判断的节点
 * @return true 节点是叶子节点
 * @return false 节点不是叶子节点
 */
bool node_is_leaf(Node *node) {
  return (node->left == NULL && node->right == NULL) ? true : false;
}

/**
 * @brief 交换堆中的两个节点
 * @param a 待交换的节点
 * @param b 待交换的节点
 */
void swap(Node **a, Node **b) {
  Node* tmp = *a;
  *a = *b;
  *b = tmp;
}