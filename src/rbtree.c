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
  node_t *p = t -> root;  // p는 t의 루트노드
  while (p != t -> nil) { // p가 리프노드 되기 전까지
    if (key == p->key) {
      // 중복된 key 삽입할 수 없으므로 리턴 
      return t -> root;
    }
    if (key < p->key) {
      // key가 루트 key보다 작으므로 왼쪽자식 탐색
      p = p->left;
    }
    else {
      // key가 루트 key보다 크므로 오른쪽자식 탐색
      p = p->right;
    }
  }

  node_t *z = (node_t*)calloc(1, sizeof(node_t));
  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // rbtree 5번 속성(모든 경로 black height 동일 )을 만족하기 위해서, 새로 삽입하는 노드의 색은 항상 red이다

  if (p == t->nil) {
    // p가 리프노드라면 empty tree
    t->root = z;
  } 
  else if (key < p->key) {
    p->left = z;
  }
  else {
    p->right = z;
  }

  // TODO: 회전하여 트리 균형 유지
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
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
