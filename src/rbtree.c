#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree)); //empty tree's root
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t)); // SENTINEL
  NIL -> color = RBTREE_BLACK;
  p -> nil = NIL;
  p -> root = NIL;
  return p;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *tmp_parent = t->nil;   // 가장 마지막으로 유효한 노드는 무조건 NIL노드 두개를 무조건 갖는다. 리프노드와 그 부모노드를 따로따로 조건탐색하지 않고 한번의 WHILE문에서 같이 저장한다.
  node_t *tmp_child = t->root;

  while (tmp_child != t -> nil) { // p가 리프노드 되기 전까지
    tmp_parent = tmp_child;
    if (key < tmp_child->key) {
      // key가 루트 key보다 작으므로 왼쪽에 들어갈 자리 탐색
      tmp_child = tmp_child->left;
    }
    else {
      // key가 루트 key보다 크거나 같으므로(MUILTISET) 오른쪽에 들어갈 자리 탐색
      tmp_child = tmp_child->right;
    }
  }
  
  node_t *z = (node_t*)calloc(1, sizeof(node_t));
  z->parent = tmp_parent;   // 마지막 리프노드로 z를 등록 
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // rbtree 5번 속성(모든 경로 black height 동일 )을 만족하기 위해서, 새로 삽입하는 노드의 색은 항상 red이다

  // TODO: 회전하여 트리 균형 유지 

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t* r = t->root;
  while (r != t->nil) {
    if key < r->
  }
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
