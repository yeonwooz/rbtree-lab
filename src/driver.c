#include "rbtree.h"
#include "rbtree.c"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {
    rbtree *p = new_rbtree();
    node_t *root = rbtree_insert(p, 50);
    // rbtree_insert(p, 20);
    // rbtree_insert(p, 60);
    // rbtree_insert(p, 10);
    // printf("%d", root -> key);
    // printf("%d", root -> left -> key);
    // printf("%d", root -> right -> key);
}