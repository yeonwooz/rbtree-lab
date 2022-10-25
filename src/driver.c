#include "rbtree.h"
#include "rbtree.c"
#include <stdio.h>
#include <stdlib.h>
int main(int argc, char *argv[]) {

    // 예시 출처 :  https://www.youtube.com/watch?v=2MdsebfJOyM
    rbtree *p = new_rbtree();
    const key_t arr[] = {20, 10, 50, 30, 80, 40, 35, 25};
    const size_t n = sizeof(arr) / sizeof(arr[0]);
    rbtree *t = new_rbtree();

    for (int i = 0; i < n; i++) {
        node_t *p = rbtree_insert(t, arr[i]);
        printf("%d\n", p->key);
    }
    printf("=============[test_find_erase] insertion done========\n");

    int len = preorder(t, t->root, 0);
    
}