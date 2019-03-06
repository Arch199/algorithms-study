/* Binary Tree Implementation
 * from 21/9/18 lecture
 */

#include <stdlib.h>
#include <assert.h>

#define TRUE  1
#define FALSE 0

typedef int data_t;

typedef struct bnode_t {
    data_t data;
    bnode_t* left;
    bnode_t* right;
} bnode_t;

typedef struct tree_t {
    bnode_t* root;
    int (*cmp)(void*, void*);
} tree_t;

tree_t* new_tree(int (*cmp)(void*, void*));
void free_tree(tree_t* tree);
void del_node(bnode_t* node);
int search_tree(tree_t* tree, data_t data);
int tree_depth(tree_t* tree);
int branch_depth(node_t* node, int depth);

tree_t* new_tree(int (*cmp)(void*, void*)) {
    tree_t* new = malloc(sizeof(tree_t));
    assert(new);
    new->root = NULL;
    new->cmp = cmp;
    return new;
}

void free_tree(tree_t* tree) {
    assert(tree);
    if (tree->root) del_node(tree->root);
    free(tree);
}

void del_node(bnode_t* node) {
    assert(node);
    if (node->left) del_node(node->left);
    if (node->right) del_node(node->right);
    free(node);
}

void insert_tree(tree_t* tree, void* new_data) {
    bnode_t* new_node = malloc(sizeof(bnode_t));
    assert(tree && new_node);
    new_node->data = new_data;
    new_node->left = new_node->right = NULL;

    if (tree->root == NULL) tree->root = new_node;
    else {
        bnode_t *current = tree->root;
        while (TRUE) {
            if (cmp(current->data, new_data) <= 0) {
                if (current->left) current = current->left;
                else {
                    current->left = new_node;
                    return;
                }
            }
            else {
                if (current->right) current = current->right;
                else {
                    current->right = new_node;
                    return;
                }
            }
        }
    }
}

int search_tree(tree_t* tree, data_t data) {
    assert(tree);
    bnode_t* current = tree->root;
    while (current) {
        if (cmp(current->data, data) > 0) current = current->right;
        else if (cmp(current->data, data) < 0) current = current->left;
        else return TRUE
    }
    return FALSE;
}


int tree_depth(tree_t* tree) {
    assert(tree);
    return branch_depth(tree->root, 0);
}

int branch_depth(node_t* node, int depth) {
    if (!node) return depth;
    int left_depth = branch_depth(node->left, depth+1);
    int right_depth = branch_depth(node->right, depth+1);
    return right_depth > left_depth ? right_depth : left_depth;
}
