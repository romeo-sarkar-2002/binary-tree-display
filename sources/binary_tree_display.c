#ifndef BINARY_TREE_DISPLAY_C
#define BINARY_TREE_DISPLAY_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layout_internal.c"
#include "doubly_linked_list_node_internal.c"
#include "queue_layout_internal.c"

#define DEBUG
// #define NO_COLOR



#if defined(NO_COLOR)
#define ELEMENT_STYLE ""
#define STYLE_RESET ""
#define NULL_ELEMENT_STYLE ""
#define LINE_STYLE ""
#define PROJECTION_LINE_STYLE ""
#else
#define ELEMENT_STYLE "\e[1;33m"
#define STYLE_RESET "\e[m"
#define NULL_ELEMENT_STYLE "\033[1;90m"
#endif


#define STRING_SIZE 8
// special characters used in drawing the branches of the tree
static char str_arr_[][3][STRING_SIZE] = 
{// {suffix,        string,     prefix}
    {"\033[1;92m",  "╭",        "\e[m"},
    {"\033[1;92m",  "╮",        "\e[m"},
    {"\033[1;91m",  "┷",        "\e[m"},
    {"\033[1;91m",  "━",        "\e[m"},
    {"\e[1;90m",    "┊",        "\e[m"},
    {"\e[1;90m",    " ",        "\e[m"}
};
#define STR_ARR_(n) str_arr_[n][0], str_arr_[n][1], str_arr_[n][2]
#define PRINT_STR_ARR_(n) printf("%s%s%s", STR_ARR_(n));
#undef STRING_SIZE




void repeat_printf(const char *_prefix, const char *_str, const char *_suffix, uint _n)
{
    printf("%s", _prefix);
    for(uint i = 0; i < _n; i++)
    {
        printf("%s", _str);
    }
    printf("%s", _suffix);
}

static void binary_tree_display_(const Layout *_layout)
{
    // level_order_traversal:
    uint max_width = 0;
    QueueLayout *queue_layout = queue_layout_create();
    queue_layout_push(queue_layout, _layout);
    while(!queue_layout_is_empty(queue_layout))
    {
        const Layout *layout = queue_layout_pop(queue_layout);
        const uint width = layout->_str_offset + layout->_str_width;
        if(max_width < width)
        {
            max_width = width;
        }
        if(!layout_is_null(layout))
        {
            // left child
            queue_layout_push(queue_layout, layout_get_left_child(layout));
            // right child
            queue_layout_push(queue_layout, layout_get_right_child(layout));
        }
    }

    // display:
    QueueLayoutNode *queue_node = queue_layout_get_head(queue_layout);
    DoublyLinkedListNode *list_head = doubly_linked_list_node_create(_layout);
    
    while(queue_node)
    {
        const uint level = queue_node->_layout->_level;

        print_1: // =>    ┊  ┊  ┊ ╭┴╮ ┊  ┊ ╭┴────────╮  ┊  
        {
            uint total_offset = 0;
            QueueLayoutNode *qnode = queue_node;
            DoublyLinkedListNode *list_node = list_head;
            
            if(0 == level)
            {
                goto exit_print_1;
            }

            _1_part_1: // ┊  ┊  ┊ ╭┴╮ ┊  ┊ ╭┴────────╮
            while(0 != qnode && qnode->_layout->_level == level)
            {
                const Layout *layout_parent = qnode->_layout->_parent;
                const Layout *layout_left_child = qnode->_layout;
                const Layout *layout_right_child = qnode->_next->_layout;

                //  ┊  ┊  ┊ 
                while(list_node->_layout != layout_parent)
                {
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // ┊
                    PRINT_STR_ARR_(4);
                    total_offset++;

                    list_node = list_node->_next;
                }

                //     ╭──┴──╮     
                {
                    // offset
                    uint offset = layout_left_child->_str_offset + layout_left_child->_str_width_left;
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // print ╭
                    PRINT_STR_ARR_(0);
                    total_offset++;
                    // ─
                    offset = layout_parent->_str_offset + layout_parent->_str_width_left;
                    repeat_printf(STR_ARR_(3), offset - total_offset);
                    total_offset = offset;
                    // ┴
                    PRINT_STR_ARR_(2);
                    total_offset++;
                    // ─
                    offset = layout_right_child->_str_offset + layout_right_child->_str_width_left;
                    repeat_printf(STR_ARR_(3), offset - total_offset);
                    total_offset = offset;
                    // ╮
                    PRINT_STR_ARR_(1);
                    total_offset++;
                }

                // update 'list_node' & 'qnode'
                list_node = list_node->_next;
                qnode = qnode->_next->_next;
            }

            _1_part_2: //  ┊  ┊  ┊ 
            {
                while(list_node)
                {
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // ┊
                    PRINT_STR_ARR_(4);
                    total_offset++;

                    list_node = list_node->_next;
                }
            }
            // align:
            repeat_printf(STR_ARR_(5), max_width - total_offset);
        }
        exit_print_1:;

        printf("\n"); // => newline


        print_2:  // =>    ┊  ┊  ┊ *┊* ┊  ┊ *      (9,9)┊  
        {
            uint total_offset = 0;
            QueueLayoutNode *qnode = queue_node;
            DoublyLinkedListNode *list_node = list_head;

            if(0 == level)
            {
                const Layout *layout_root = queue_node->_layout;
                uint offset = layout_root->_str_offset;
                // offset
                repeat_printf(STR_ARR_(5), offset);
                // element / null-element
                if(layout_is_null(layout_root))
                {
                    printf(NULL_ELEMENT_STYLE "%s" STYLE_RESET, layout_root->_str);
                }
                else
                {
                    printf(ELEMENT_STYLE "%s" STYLE_RESET, layout_root->_str);
                }
                offset += layout_root->_str_width;

                // align:
                repeat_printf(STR_ARR_(5), max_width - offset);
                
                // update 'queue_node'
                queue_node = queue_node->_next;
                goto exit_print_2;
            }
            
            _2_part_1: //    ┊  ┊  ┊ *┊* ┊  ┊ *      (9,9)
            while(0 != qnode && qnode->_layout->_level == level)
            {
                const Layout *layout_parent = qnode->_layout->_parent;
                const Layout *layout_left_child = qnode->_layout;
                const Layout *layout_right_child = qnode->_next->_layout;
                
                //  ┊  ┊  ┊ 
                while(list_node->_layout != layout_parent)
                {
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // ┊
                    PRINT_STR_ARR_(4);
                    total_offset++;

                    list_node = list_node->_next;
                }

                // *┊(2,7)
                {
                    // offset
                    uint offset = layout_left_child->_str_offset;
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // string
                    if(layout_is_null(layout_left_child))
                    {
                        printf(NULL_ELEMENT_STYLE "%s" STYLE_RESET, layout_left_child->_str);
                    }
                    else
                    {
                        printf(ELEMENT_STYLE "%s" STYLE_RESET, layout_left_child->_str);
                    }
                    total_offset += layout_left_child->_str_width;
                    // offset
                    offset = layout_parent->_str_offset + layout_parent->_str_width_left;
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // ┊
                    PRINT_STR_ARR_(4);
                    total_offset++;
                    // offset
                    offset = layout_right_child->_str_offset;
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // string
                    if(layout_is_null(layout_right_child))
                    {
                        printf(NULL_ELEMENT_STYLE "%s" STYLE_RESET, layout_right_child->_str);
                    }
                    else
                    {
                        printf(ELEMENT_STYLE "%s" STYLE_RESET, layout_right_child->_str);
                    }
                    total_offset += layout_right_child->_str_width;

                }

                // updating 'list_node' & 'qnode'
                DoublyLinkedListNode *previous_list_node = list_node;
                list_node = list_node->_next;
                doubly_linked_list_node_expand(previous_list_node);
                qnode = qnode->_next->_next;
            }

            _2_part_2://  ┊  ┊  ┊ 
            {
                while(list_node)
                {
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    repeat_printf(STR_ARR_(5), offset - total_offset);
                    total_offset = offset;
                    // ┊
                    PRINT_STR_ARR_(4);
                    total_offset++;
                    list_node = list_node->_next;
                }
            }
            // align:
            repeat_printf(STR_ARR_(5), max_width - total_offset);

            // update 'queue_node'
            queue_node = qnode;
        }
        exit_print_2:;

        printf("\n"); // => newline

        // update 'list_head'
        list_head = doubly_linked_list_node_get_head(list_head);

    }

    
    queue_layout_destroy(queue_layout);
    doubly_linked_list_node_destroy_recursive(list_head);
}

void binary_tree_display(void *_node, const void *(*_func_get_left_child)(const void *node), const void *(*_func_get_right_child)(const void *_node), char *(*_func_get_string)(const void *_node))
{
    // setting the function pointers;
    layout_set_func_get_left_child(_func_get_left_child);
    layout_set_func_get_right_child(_func_get_right_child);
    layout_set_func_get_string(_func_get_string);

    Layout *l = layout_generate(_node);

    binary_tree_display_(l);

    layout_destroy(l);
}


#endif