#include "binary_tree_node.c"
#include "sources/binary_tree_display.c"

int main()
{
    const char *arr_1[] = {"C/C++", "CP", "DSA", "linux", NULL, NULL, "convex hull", "binary", "code", "n-ary trees", "not_null", NULL, "ternary search", NULL, "developer", NULL, "trie", "graphs", "DSU", NULL, NULL, "Automata", "DP", "loop"};
    const char *arr_2[] = {"(1,2)", "(2,3)", "(12,1)", "(6,4)", "(1,8)", 0, "(6,3)", 0, 0, 0, "(2,7)", "(1237,1)", 0, 0, 0, 0, "(9,99)", "(4,9)", 0, 0, "(3,7)"};
    
    uint size_1 = sizeof arr_1 / sizeof(char *);
    uint size_2 = sizeof arr_2 / sizeof(char *);
    
    BinaryTreeNode *tree_root_1 = binary_tree_node_create_recursive_arr(arr_1, size_1);
    BinaryTreeNode *tree_root_2 = binary_tree_node_create_recursive_arr(arr_2, size_2);

    binary_tree_display(tree_root_1, binary_tree_node_get_left_child, binary_tree_node_get_right_child, binary_tree_node_get_string);
    binary_tree_display(tree_root_2, binary_tree_node_get_left_child, binary_tree_node_get_right_child, binary_tree_node_get_string);

    binary_tree_node_destroy_recursive(tree_root_1);
    binary_tree_node_destroy_recursive(tree_root_2);
    
    return 0;
}