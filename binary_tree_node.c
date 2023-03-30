#include <stdio.h>
#include <stdlib.h>
#include "data.c"

typedef struct BinaryTreeNode BinaryTreeNode;
BinaryTreeNode *binary_tree_node_create(struct Data *_data);
const void *binary_tree_node_get_left_child(const void *_node);
const void *binary_tree_node_get_left_child(const void *_node);



typedef struct BinaryTreeNode
{
    Data                *_data;
    BinaryTreeNode      *_left;
    BinaryTreeNode     *_right;
} BinaryTreeNode;

struct BinaryTreeNode *binary_tree_node_create(Data *_data)
{
    BinaryTreeNode *binary_tree_node = malloc(sizeof(BinaryTreeNode));
    binary_tree_node->_data = _data;
    binary_tree_node->_right = binary_tree_node->_left = 0;
    return binary_tree_node;
}
const void *binary_tree_node_get_left_child(const void *_node)
{
    const BinaryTreeNode *binary_tree_node = _node;
    return binary_tree_node->_left;
}
const void *binary_tree_node_get_right_child(const void *_node)
{
    const BinaryTreeNode *binary_tree_node = _node;
    return binary_tree_node->_right;
}
char *binary_tree_node_get_data(const void *n)
{
    if(0 == n)
    {
    #define LENGTH 32
        char *p = malloc(sizeof(char) * ( LENGTH + 1 ));
        snprintf(p, LENGTH, "*");
        p[LENGTH] = 0;
    #undef LENGTH
        return p;
    }
    const struct BinaryTreeNode *node = n;
    return data_to_string(node->_data);
}

void binary_tree_node_traverse_inorder(const struct BinaryTreeNode *n)
{
    if(0 == n)return;
    binary_tree_node_traverse_inorder(binary_tree_node_get_left_child(n));
    printf("\"%s\"", binary_tree_node_get_data(n));
    binary_tree_node_traverse_inorder(binary_tree_node_get_right_child(n));
}

void binary_tree_node_destroy(BinaryTreeNode *_node)
{
    free(_node);
}
void binary_tree_node_destroy_recursive(BinaryTreeNode *_node)
{
    if(0 == _node)
        return;
    if(_node->_left)
    {
        binary_tree_node_destroy_recursive(_node->_left);
    }
    if(_node->_right)
    {
        binary_tree_node_destroy_recursive(_node->_right);
    }
    binary_tree_node_destroy(_node);
}