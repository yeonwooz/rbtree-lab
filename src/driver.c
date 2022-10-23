#include "rbtree.h"
#include "rbtree.c"
#include <stdio.h>

int main(int argc, char *argv[]) {
    rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
    node_t *root = rbtree_insert(p, 12);
    printf("%d", root -> key);
}