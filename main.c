#include "binary_tree_node.c"
#include "sources/binary_tree_display.c"
int main()
{
    struct BinaryTreeNode *n = binary_tree_node_create(data_create(1, 2));
    n->_left = binary_tree_node_create(data_create(2, 3));
    n->_left->_left = binary_tree_node_create(data_create(13, 4));
    // n->_left->_left->_left=n->_left->_left->_right=0;
    n->_left->_right = binary_tree_node_create(data_create(1, 8));
    // n->_left->_right->_left=0;
    n->_left->_right->_right = binary_tree_node_create(data_create(2, 7));

    n->_right = binary_tree_node_create(data_create(4, 1));
    n->_right->_right = binary_tree_node_create(data_create(6, 3));
    n->_right->_right->_left = binary_tree_node_create(data_create(7, 1));
    n->_right->_right->_left->_right = binary_tree_node_create(data_create(9, 9));
    n->_right->_right->_left->_right->_left = binary_tree_node_create(data_create(4, 9));
    n->_right->_right->_left->_right->_left->_right = binary_tree_node_create(data_create(3, 7));


    binary_tree_display(n, binary_tree_node_get_left_child, binary_tree_node_get_right_child, binary_tree_node_get_data);
    // goto *0;
    return 0;
}