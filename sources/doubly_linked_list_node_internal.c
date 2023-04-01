#ifndef DOUBLY_LINKED_LIST_C
#define DOUBLY_LINKED_LIST_C

#include <stdio.h>
#include <stdlib.h>
#include "layout_internal.c"
#include "error_internal.h"

// declaration
typedef struct DoublyLinkedListNode DoublyLinkedListNode;

DoublyLinkedListNode   *doubly_linked_list_node_create(const Layout *_layout);
void                    doubly_linked_list_node_destroy_recursive(DoublyLinkedListNode *_node);

void                    doubly_linked_list_node_expand(DoublyLinkedListNode *_node);
void                    doubly_linked_list_node_print(DoublyLinkedListNode *_head);

DoublyLinkedListNode   *doubly_linked_list_node_get_head(DoublyLinkedListNode *_node);


typedef struct DoublyLinkedListNode // doubly linked list
{
    const Layout                    *_layout;
    struct DoublyLinkedListNode   *_previous;
    struct DoublyLinkedListNode       *_next;
} DoublyLinkedListNode;


DoublyLinkedListNode *doubly_linked_list_node_create(const Layout *_layout)
{
    DoublyLinkedListNode *node = (DoublyLinkedListNode *)malloc(sizeof(DoublyLinkedListNode));
    ERROR(doubly_linked_list_node_create, 0 == node, "malloc returned (null)", return 0);
    node->_previous = node->_next = 0;
    node->_layout = _layout;
    return node;
}

static void doubly_linked_list_node_destroy(DoublyLinkedListNode *_node)
{
    if(0 == _node)
    {
        return;
    }
    free(_node);
}

void doubly_linked_list_node_destroy_recursive(DoublyLinkedListNode *_node)
{
    if(0 == _node)
    {
        return;
    }
    // deleting all previous nodes
    DoublyLinkedListNode *previous = _node->_previous;
    while(previous)
    {
        DoublyLinkedListNode *removed_node = previous;
        previous = previous->_previous;
        doubly_linked_list_node_destroy(removed_node);
    }
    // deleting all next nodes
    DoublyLinkedListNode *next = _node->_next;
    while(next)
    {
        DoublyLinkedListNode *removed_node = next;
        next = next->_next;
        doubly_linked_list_node_destroy(removed_node);
    }
    // deleting the current node
    doubly_linked_list_node_destroy(_node);
}

static void doubly_linked_list_node_expand_left(DoublyLinkedListNode *_node)
{
    ERROR(doubly_linked_list_node_expand_left, 0 == _node, "_node is (null)", return);
    const Layout *layout = layout_get_left_child(_node->_layout);
    if(layout_is_null(layout))
    {
        return;
    }
    DoublyLinkedListNode *previous = _node->_previous, *new_node = doubly_linked_list_node_create(layout);
    new_node->_previous = previous;
    new_node->_next = _node;
    _node->_previous = new_node;
    if(previous)
    {
        previous->_next = new_node;
    }
}

static void doubly_linked_list_node_expand_right(DoublyLinkedListNode *_node)
{
    ERROR(doubly_linked_list_node_expand_left, 0 == _node, "_node is (null)", return);
    const Layout *layout = layout_get_right_child(_node->_layout);
    if(layout_is_null(layout))
    {
        return;
    }
    DoublyLinkedListNode *next = _node->_next, *new_node = doubly_linked_list_node_create(layout);
    new_node->_previous = _node;
    new_node->_next = next;
    _node->_next = new_node;
    if(next)
    {
        next->_previous = new_node;
    }
}

void doubly_linked_list_node_expand(DoublyLinkedListNode *_node)
{
    doubly_linked_list_node_expand_left(_node);
    doubly_linked_list_node_expand_right(_node);
}

DoublyLinkedListNode *doubly_linked_list_node_get_head(DoublyLinkedListNode *_node)
{
    ERROR(doubly_linked_List_head, 0 == _node, "_node is (null)", return 0);
    while(_node->_previous)
    {
        _node = _node->_previous;
    }
    return _node;
}

void doubly_linked_list_node_print(DoublyLinkedListNode *_head)
{
    printf("[");
    while(_head)
    {
        printf("\"\e[1;4;33m%s\e[m\"", _head->_layout->_str);
        if(_head->_next)
        {
            printf("\e[1;31m<=>\e[m");
        }
        _head = _head->_next;
    }
    printf("]");
}

#endif