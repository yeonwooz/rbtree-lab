#include "rbtree.h"
#include <stdio.h>
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
  if (t->root == t->nil) {
    free(t->nil);
    t=NULL;
    return;
  }

  if (t->root->left != t->nil) {
    free(t->root->left);
  }

  if (t->root->right != t->nil) {
    free(t->root->right);
  }

  free(t->nil);
  free(t);
  t=NULL;
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

      node_t *uncle = z->parent->parent->left;
      
      if (uncle->color == RBTREE_RED) {
        // 만약 g_parent의 두 자식노드가 모두 레드라면, 부모와 자식들의 색을 교환하여 RULE5를 유지시켜준다.

        z->parent->parent->color = RBTREE_RED;
        
        z->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;   

        z = z->parent->parent;
        // parent 노드 색상 변경으로 인해 g_parent노드에 위반이 발생했는지 여부를 확인(WHILE LOOP)
      }
      else {
        // uncle의 색이 블랙일 때
        if (z == z->parent->left)  {
          // g_p - p - z가 삼각형 모양을 이룬다면 일단 펴주기 위해 right_rotate 한다{
          z= z->parent;
          right_rotate(t, z);
          // 레드를 기준으로 그 아래의 블랙노드와 함께 회전시키므로 위쪽이 블랙, 아래쪽이 레드가 된다. 
        }

        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
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

  rbtree_insert_fixup(t, z);  // TODO: insertion 4번째에서 문제발생중
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

node_t *rbtree_successor(const rbtree *t, node_t *z) {
  // 맨 왼쪽 자식으로 끝까지 타고 내려가서 min을 리턴하는 함수 
  node_t *temp_parent;
  node_t *temp_child = z;
  // z 를 기준으로 min 찾기 (successor 찾기)

  while (temp_child != t->nil) {
    temp_parent = temp_child;
    temp_child = temp_child->left;
  }
  return temp_parent;
}

node_t *rbtree_predecessor(const rbtree *t, node_t *z) {
  // 맨 오른쪽 자식으로 끝까지 타고 내려가서 min을 리턴하는 함수 
  node_t *temp_parent;
  node_t *temp_child = z;
  // z 를 기준으로 min 찾기 (successor 찾기)

  while (temp_child != t->nil) {
    temp_parent = temp_child;
    temp_child = temp_child->right;
  }
  return temp_parent;
}
node_t *rbtree_min(const rbtree *t) {
  return rbtree_successor(t, t->root);
}

node_t *rbtree_max(const rbtree *t) {
  return rbtree_predecessor(t, t->root);
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

void rbtree_erase_fixup(rbtree *t, node_t *x) {
  // node_t* x 는 extra-black노드

  if (x == t->nil) {
    return;
  }
  node_t* w; // extra-black노드의 형제노드

  // x->color가 레드면 블랙으로 교체만 해주면 된다 
  while (x != t->root && x->color == RBTREE_BLACK) {
    // x는 doubly-black노드 

    // while문을 통해 각 case들의 extra-black이 존재하는 노드들이 부모레벨의 노드들과 회전 및 재배치되어 결국 case 4로 수렴하고, case4를 해결함으로써 while문 종료

    if (x == x->parent->left) {
      // doubly-black(d.b)이 부모의 왼쪽자식일 때 
      w=x->parent->right;

      if (w->color == RBTREE_RED) {
        // CASE1: w=RED

        // 우선 d.b의 부모와 형제의 색 교환
        w->color = RBTREE_BLACK;   
        x->parent->color = RBTREE_RED;     
        
        // d.b의 부모 기준으로 왼쪽 회전
        left_rotate(t, x->parent);

        w = x->parent->right;  // 회전 후 doubly-black노드의 형제는 d.b->parent->right로 이동해있다.
      }

      // CASE1에서 d.b(x)의 형제(w)를 black으로 만들면, CASE 2/3/4 중 하나가 된다.
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        // CASE2: w=BLACK && w->l=BLACK && w->r=BLACK

        // w의 두 자식노드가 모두 블랙이면
        w->color = RBTREE_RED;
        x = x->parent;
      }
      else {
        // CASE 3: w=BLACK && w->l=RED
        // w->l의 red를 w->r로 보내주어 CASE 4로 수렴시키고 CASE 4를 해결
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          right_rotate(t, w);
          w = x->parent->right;
        

        // CASE 4: w=BLACK && w->r=RED
        
        // d.b의 형제와 부모 색상 교체 (결국 다시 좌우반전되면서 부모의 색은 원래대로 돌아올 예정)
        w->color = x->parent->color;   
        x->parent->color = RBTREE_BLACK;

        // d.b의 형제의 오른쪽 노드 색상 교체후 왼쪽 회전으로 트리 반전
        w->right->color = RBTREE_BLACK;
        left_rotate(t, x->parent);

        // CASE 4 해결. 트리의 extra-black이 모두 사라졌으므로 while문 종료 (r&b가 된 x노드만 BLACK으로 색칠하고 fixup함수 종료)
        x=t->root;  
        }
      }
    }
    else {
      // doubly-black이 부모의 오른쪽자식일 때 
      w=x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w=x->parent->left;
      }

      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      }

      else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w=x->parent->left;
        }

        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        x->parent->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        
        x=t->root;
      }
    }
  }

  x->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *z) {
  node_t *y = z;
  node_t *x = t->nil;  // initialization
  color_t y_original_color = y->color;

  // z의 자식이 없거나 하나만 있다면, z와 부모노드의 연결관계를 z의 자식과의 관계로 교체한다.
  if (z->left == t->nil) {
    x=z->right;
    // x에 z의 오른쪽 서브트리 저장
    rbtree_transplant(t, z, z->right);
  }
  else if (z->right == t->nil) {
    x=z->left;
    // x에 z의 왼쪽 서브트리 저장
    rbtree_transplant(t, z, z->left);
  }
  else {
    // z가 자식이 두개라면, successor를 찾아준다.
    y = rbtree_successor(t, z->right);
    // z의 오른쪽 서브트리 기준 min값이 successor가 된다 
    y_original_color = y->color;
    
    if (y != z->right) {
      // successor 가 y의 오른쪽 노드에서 한참 더 내려갔다면
      
      rbtree_transplant(t, y, y->right);
      y->right = z->right;
      y->right->parent = y;
      // y대신 y->right으로 삭제할 노드 z의 부모노드와의 연결관계를 교체
    }
    else {
      // successor 가 바로 y의 오른쪽 노드라면
      x->parent = y;
      //  x->parent 를 y로 저장 (부모자식 노드 될 예정)
    }

    rbtree_transplant(t, z, y);
    y->left = z->left;
    y->left->parent = y;
    // z를 y로 교체

    y->color = z->color;
    // z의 자리를 차지한 y노드의 색은 삭제된 노드의 색상을 물려받는다.
  }

  if (y_original_color == RBTREE_BLACK) {
    // 만약 삭제할 노드의 색이 레드였다면 여기서 종료할 수 있다.
    // 만약 삭제할 노드의 색이 검정이었다면 후작업 필요

    // x는 extra-black 을 갖는 노드
    rbtree_erase_fixup(t, x);
  }

  return 0;
}

int preorder(rbtree *t, node_t *cur_node, int len) {
  if (cur_node == t->nil) {
    return len;
  }
  printf("key=%d\n", cur_node->key);
  len = len + 1;
  len = preorder(t, cur_node->left, len);
  len = preorder(t, cur_node->right, len);

  return len;
}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // 부모와 두 자식의 인덱스 관계는 i, 2 * i + 1, 2 * i + 2  이다.
  int len = preorder(t, t->root, 0);
  key_t *keys = (key_t *)calloc(len, sizeof(key_t));
  for (int i = 0; i < len; i++) {

  }

  return 0;
}
