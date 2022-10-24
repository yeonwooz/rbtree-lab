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

void left_rotate(rbtree *t, node_t *x) {
  // 1. 기준노드 x와, x의 오른쪽 자식노드 연결관계를 끊는다.
  node_t *y = x->right;  // temporary saved
  x->right = y->left;

  // 1-2. x의 오른쪽 자식노드가 혹시 왼쪽 노드(혹은 서브트리)를 갖는다면 이를 x에 연결시킨다.
  if (y->left != t->nil) {
    y->left->parent = x;
  }

  // 2. x의 오른쪽 자식노드를 할아버지 노드에 연결시킨다.
  y->parent = x->parent;  
  
  // 2-2. x->parent 기준으로 자식노드들 재배치
  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->left = x;

  // 3. x를 y에 연결시킨다.
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;

  if (y->right != t->nil)
    y->right->parent = x;

  y->parent = x->parent;

  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->right) {
    x->parent->right = y;
  }
  else {
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (1) {  
    if (z->parent->color == RBTREE_BLACK) {
      // 부모노드가 블랙이므로 블랙-레드가 되어 위반사항 없음.
      // z가 루트노드일 경우에도, 부모가 Nil일 것이므로 색상은 블랙.
      break;
    }
    // 방금 삽입한 z가 레드이므로 부모가 레드라면 RULE4위반 
    if (z->parent == z->parent->parent->left){
      // parent가 g_parent의 좌노드일 때

      node_t *uncle = z->parent->parent->right; //y = uncle을 의미함
      if (uncle->color == RBTREE_RED) {
        // uncle의 색이 레드일 때
        // parent가 red 이므로 g_parent는 블랙일 것이다.
        // 만약 g_parent의 두 자식노드가 모두 레드라면, 부모와 자식들의 색을 교환하여 RULE5를 유지시켜준다.
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent; // parent 노드 색상 변경으로 인해 g_parent노드에 위반이 발생했는지 여부를 확인(WHILE LOOP)
      }
      else {
        // uncle의 색이 블랙일 때
        if (z == z->parent->right) {
          // g_p - p - z가 삼각형 모양을 이룬다면 일단 펴주기 위해 left_rotate 한다
          z = z->parent;  // 기준을 부모노드로 변경
          left_rotate(t,z);
        }
        z->parent->parent->color = RBTREE_RED;
        z->parent->color = RBTREE_BLACK;
        right_rotate(t, z->parent->parent); 
        // 레드를 기준으로 그 아래의 블랙노드와 함께 회전시키므로 위쪽이 블랙, 아래쪽이 레드가 된다. 
      }
    } 
    else {
      // parent가 g_parent의 우노드일 때
    }
  }
  
  if (t->root->color == RBTREE_RED) {
    // RULE2 위반 해결
    t->root->color = RBTREE_BLACK;
  }
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

  if (z->parent == t->nil)
    t->root = z;
  else if (key < z->parent->key)
    z->parent->left = z;
  else
    z->parent->right = z;

  z->key = key;
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED; // rbtree 5번 속성(모든 경로 black height 동일 )을 만족하기 위해서, 새로 삽입하는 노드의 색은 항상 red이다.

  rbtree_insert_fixup(t, z);
  return z;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t* r = t->root;
  
  while (r != t->nil) {
    if (key < r->key) {
      r = r->left;
    } 
    else if (key > r->key) {
      r = r->right;
    }
    else {
      return r;
    }
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  } 
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase

  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
