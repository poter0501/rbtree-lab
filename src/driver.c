#include "rbtree.h"

int main(int argc, char *argv[])
{
    rbtree *r = new_rbtree();
    rbtree_insert(r, 5);
    rbtree_insert(r, 3);
    rbtree_insert(r, 7);
    rbtree_insert(r, 2);
    rbtree_insert(r, 4);
    rbtree_insert(r, 6);
    rbtree_insert(r, 8);

    delete_rbtree(r);
    // // rotate_right(r, r->root->left);
    // // rotate_right(r, r->root);
    // node_t *temp1;
    // temp1= rbtree_find(r, 6);
    // node_t *temp2;
    // temp2= rbtree_find(r, 10);
    // return 0;
}