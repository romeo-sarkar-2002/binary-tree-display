#include "binary_tree_node.c"
#include "sources/binary_tree_display.c"

int main()
{
    const char *arr[] = {"abc", "bs", "c", "cd", NULL, NULL, "tr", "binary", "ee", "ary", "null", NULL, "xs", NULL, "developer", NULL, "trie", "graphs", "DSU", NULL, NULL, "cs", "DP", "loop"};
    uint size = sizeof arr / sizeof(char *);
    
    BinaryTreeNode *tree_root = binary_tree_node_create_recursive_arr(arr, size);


    binary_tree_display(tree_root, binary_tree_node_get_left_child, binary_tree_node_get_right_child, binary_tree_node_get_string);

    binary_tree_node_destroy_recursive(tree_root);
    
    printf("\nall done\n");
    return 0;
}