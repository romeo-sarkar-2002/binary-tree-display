#ifndef BINARY_TREE_NODE_H
#define BINARY_TREE_NODE_H

#include <stdint.h>
typedef struct BinaryTreeNode BinaryTreeNode;
typedef uint32_t uint;

BinaryTreeNode *binary_tree_node_create_recursive(uint _n, ...);
void            binary_tree_node_destroy_recursive(BinaryTreeNode *_node);

void binary_tree_node_traverse_inorder(const struct BinaryTreeNode *_node);

const void *binary_tree_node_get_left_child(const void *_node);
const void *binary_tree_node_get_right_child(const void *_node);
char       *binary_tree_node_get_string(const void *n);


#endif