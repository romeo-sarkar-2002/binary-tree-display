#ifndef BINARY_TREE_DISPLAY
#define BINARY_TREE_DISPLAY

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "layout.c"
#include "doubly_linked_list_node.c"
#include "queue_layout.c"

#define DEBUG
#define NO_COLOR



#if defined(NO_COLOR)
#define ELEMENT_STYLE ""
#define STYLE_RESET ""
#define NULL_ELEMENT_STYLE ""
#define LINE_STYLE ""
#define PROJECTION_LINE_STYLE ""
#else
#define ELEMENT_STYLE "\e[1;32m"
#define STYLE_RESET "\e[m"
#define NULL_ELEMENT_STYLE "\033[1;90m"
#define LINE_STYLE "\033[1;31m"
#define PROJECTION_LINE_STYLE "\e[1;90m"
#endif


#define STRING_SIZE 8
// special characters used in drawing of the tree
static const char str_arr_[][STRING_SIZE] = {"╭","╮","┷","━","┊"," "};
#undef STRING_SIZE


void repeat_printf(const char *_s, uint _n)
{
    for(uint i = 0; i < _n; i++)
    {
        printf("%s", _s);
    }
}

static void binary_tree_display_(const Layout *_layout)
{
    // level_order_traversal:
    uint max_width = 0;
    QueueLayout *queue = queue_layout_create();
    queue_layout_push(queue, _layout);
    while(!queue_layout_is_empty(queue))
    {
        const Layout *layout = queue_layout_pop(queue);
        const uint width = layout->_str_offset + layout->_str_width;
        if(max_width < width)
        {
            max_width = width;
        }
        if(!layout_is_null(layout))
        {
            // left child
            queue_layout_push(queue, layout_get_left_child(layout));
            // right child
            queue_layout_push(queue, layout_get_right_child(layout));
        }
    }

    // display
    QueueLayoutNode *queue_node = queue_layout_head(queue);
    DoublyLinkedListNode *list_head = doubly_linked_list_node_create(_layout);
    while(queue_node)
    {
        // doubly_linked_list_node_print(list_head);
        const uint level = queue_node->_layout->_level;

        // print_1 =>    ┊  ┊  ┊ ╭┴╮ ┊  ┊ ╭┴────────╮  ┊  
        if(0 < level)
        {
            uint total_offset = 0;
            QueueLayoutNode *qnode = queue_node;
            DoublyLinkedListNode *list_node = list_head;
            while(0 != qnode && qnode->_layout->_level == level)
            {
                const Layout *layout_parent = qnode->_layout->_parent;
                const Layout *layout_left_child = qnode->_layout;
                const Layout *layout_right_child = qnode->_next->_layout;
                
                //  ┊  ┊  ┊ 
                while(list_node->_layout != layout_parent)
                {
                    // printf("list_node = (%p, %s), layout_parent = (%p, %s)\n", list_node->_layout, list_node->_layout->_str, layout_parent, layout_parent->_str);
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    if(total_offset < offset)
                    {
                        repeat_printf(str_arr_[5], offset - total_offset);
                        total_offset = offset;
                    }
                    // ┊
                    printf(str_arr_[4]);
                    total_offset++;
                    list_node = list_node->_next;
                }
                
                //     ╭──┴──╮     
                {
                    // offset
                    uint offset = layout_left_child->_str_offset + layout_left_child->_str_width_left;
                    repeat_printf(str_arr_[5], offset - total_offset);
                    total_offset = offset;
                    
                    // print ╭
                    printf(str_arr_[0]);
                    total_offset++;
                    
                    // ─
                    offset = layout_parent->_str_offset + layout_parent->_str_width_left;
                    repeat_printf(str_arr_[3], offset - total_offset);
                    total_offset = offset;
                    
                    // ┴
                    printf(str_arr_[2]);
                    total_offset++;
                    
                    // ─
                    offset = layout_right_child->_str_offset + layout_right_child->_str_width_left;
                    repeat_printf(str_arr_[3], offset - total_offset);
                    total_offset = offset;
                    
                    // ╮
                    printf(str_arr_[1]);
                    total_offset++;
                }

                list_node = list_node->_next;
                qnode = qnode->_next->_next;
            }
            
            //  ┊  ┊  ┊ 
            {
                while(list_node)
                {
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    if(total_offset < offset)
                    {
                        repeat_printf(str_arr_[5], offset - total_offset);
                        total_offset = offset;
                    }
                    // ┊
                    printf(str_arr_[4]);
                    total_offset++;
                    list_node = list_node->_next;
                }
            }
            // align:
            repeat_printf(str_arr_[5], max_width - total_offset);

            printf("\n");
        }

        // print_2 =>   *┊* ┊ *┊(2,7)┊  ┊(7,1)         ┊* 
        if(0 < level)
        {
            uint total_offset = 0;
            QueueLayoutNode *qnode = queue_node;
            DoublyLinkedListNode *list_node = list_head;
            while(0 != qnode && qnode->_layout->_level == level)
            {
                const Layout *layout_parent = qnode->_layout->_parent;
                const Layout *layout_left_child = qnode->_layout;
                const Layout *layout_right_child = qnode->_next->_layout;
                
                //  ┊  ┊  ┊ 
                if(0 < level)
                {
                    while(list_node->_layout != layout_parent)
                    {
                        // printf("list_node = (%p, %s), layout_parent = (%p, %s)\n", list_node->_layout, list_node->_layout->_str, layout_parent, layout_parent->_str);
                        const Layout *layout = list_node->_layout;
                        uint offset = layout->_str_offset + layout->_str_width_left;
                        // offset
                        if(total_offset < offset)
                        {
                            repeat_printf(str_arr_[5], offset - total_offset);
                            total_offset = offset;
                        }
                        // ┊
                        printf(str_arr_[4]);
                        total_offset++;
                        list_node = list_node->_next;
                    }
                    DoublyLinkedListNode *previous_list_node = list_node;
                    list_node = list_node->_next;
                    doubly_linked_list_node_expand(previous_list_node);

                }
                
                // *┊(2,7)
                {
                    // offset
                    uint offset = layout_left_child->_str_offset;
                    if(total_offset < offset)
                    {
                        repeat_printf(str_arr_[5], offset - total_offset);
                        total_offset = offset;
                    }
                    // string
                    printf("%s", layout_left_child->_str);
                    total_offset += layout_left_child->_str_width;
                    // offset
                    offset = layout_parent->_str_offset + layout_parent->_str_width_left;
                    if(total_offset < offset)
                    {
                        repeat_printf(str_arr_[5], offset - total_offset);
                        total_offset = offset;
                    }
                    // ┊
                    printf(str_arr_[4]);
                    total_offset++;
                    // offset
                    offset = layout_right_child->_str_offset;
                    if(total_offset < offset)
                    {
                        repeat_printf(str_arr_[5], offset - total_offset);
                        total_offset = offset;
                    }
                    // string
                    printf("%s", layout_right_child->_str);
                    total_offset += layout_right_child->_str_width;


                    qnode = qnode->_next->_next;
                }

            }

            //  ┊  ┊  ┊ 
            {
                while(list_node)
                {
                    const Layout *layout = list_node->_layout;
                    uint offset = layout->_str_offset + layout->_str_width_left;
                    // offset
                    if(total_offset < offset)
                    {
                        repeat_printf(str_arr_[5], offset - total_offset);
                        total_offset = offset;
                    }
                    // ┊
                    printf(str_arr_[4]);
                    total_offset++;
                    list_node = list_node->_next;
                }
            }

            if(total_offset < max_width)
            {
                repeat_printf(str_arr_[5], max_width - total_offset);
            }

            queue_node = qnode;
            // printf("\n"); // => newline
        }
        else
        {
            const Layout *layout_root = queue_node->_layout;
            uint offset = layout_root->_str_offset;

            repeat_printf(str_arr_[5], offset);

            printf("%s", layout_root->_str);

            if(offset < max_width)
            {
                repeat_printf(str_arr_[5], max_width - offset);
            }
            queue_node = queue_node->_next;
            // printf("\n"); // => newline
        }

        

        list_head = doubly_linked_list_node_head(list_head);

        // doubly_linked_list_node_print(list_head);

        printf("\n"); // => newline
    }

}

void binary_tree_display(void *_node, const void *(*_func_get_left_child)(const void *node), const void *(*_func_get_right_child)(const void *_node), char *(*_func_get_string)(const void *_node))
{
    // setting the function pointers;
    get_left_child_ = _func_get_left_child;
    get_right_child_ = _func_get_right_child;
    get_string_ = _func_get_string;
    Layout *l = layout_generate(_node);

    // layout_print
    layout_traverse_inorder(l);

    binary_tree_display_(l);


    layout_destroy(l);
}



#endif