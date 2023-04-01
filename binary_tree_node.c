#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "binary_tree_node.h"

// declaration
typedef struct Data Data;

Data *data_create(const char *_str);
char *data_to_string(Data *_data);


typedef struct Data
{
    char *_str;
} Data;

Data *data_create(const char *_str)
{
    if(0 == _str)
    {
        return 0;
    }
    Data *node = (Data *)malloc(sizeof(Data));
    int length = strlen(_str);
    node->_str = (char *)malloc(sizeof(char) * length);
    strcpy(node->_str, _str);
    return node;
}

// claim ownership of char *
char *data_to_string(Data *_data)
{
#define LENGTH 32
    char *p = (char *)malloc(sizeof(char) * (LENGTH + 1));
    snprintf(p, LENGTH, "%s", _data->_str);
    p[LENGTH] = 0;
#undef LENGTH
    return p;
}

typedef struct BinaryTreeNode
{
    Data                *_data;
    BinaryTreeNode      *_left;
    BinaryTreeNode     *_right;
} BinaryTreeNode;

static struct BinaryTreeNode *binary_tree_node_create(Data *_data)
{
    if(0 == _data)
    {
        return 0;
    }
    BinaryTreeNode *binary_tree_node = (BinaryTreeNode *)malloc(sizeof(BinaryTreeNode));
    binary_tree_node->_data = _data;
    binary_tree_node->_right = 0;
    binary_tree_node->_left = 0;
    return binary_tree_node;
}

const void *binary_tree_node_get_left_child(const void *_node)
{
    const BinaryTreeNode *binary_tree_node = (BinaryTreeNode *)_node;
    return binary_tree_node->_left;
}
const void *binary_tree_node_get_right_child(const void *_node)
{
    const BinaryTreeNode *binary_tree_node = (BinaryTreeNode *)_node;
    return binary_tree_node->_right;
}
char *binary_tree_node_get_string(const void *_node)
{
    if(0 == _node)
    {
    #define LENGTH 32
        char *p = (char *)malloc(sizeof(char) * (LENGTH + 1));
        snprintf(p, LENGTH, "");
        p[LENGTH] = 0;
    #undef LENGTH
        // return 0;
        return p;
    }
    const struct BinaryTreeNode *node = (BinaryTreeNode *)_node;
    return data_to_string(node->_data);
}


void binary_tree_node_traverse_inorder(const struct BinaryTreeNode *_node)
{
    if(0 == _node)
    {
        return;
    }
    binary_tree_node_traverse_inorder((const BinaryTreeNode *)binary_tree_node_get_left_child(_node));
    printf("\"%s\" ", binary_tree_node_get_string(_node));
    binary_tree_node_traverse_inorder((const BinaryTreeNode *)binary_tree_node_get_right_child(_node));
}

static void binary_tree_node_destroy(BinaryTreeNode *_node)
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


// declaration
typedef struct QueueBinaryTreeNodeNode QueueBinaryTreeNodeNode;

QueueBinaryTreeNodeNode *queue_binary_tree_node_node_create(BinaryTreeNode *_node);
void queue_binary_tree_node_node_destroy(QueueBinaryTreeNodeNode *_queue_node);


typedef struct QueueBinaryTreeNodeNode
{
    BinaryTreeNode *_node;
    struct QueueBinaryTreeNodeNode *_next;
} QueueBinaryTreeNodeNode;

QueueBinaryTreeNodeNode *queue_binary_tree_node_node_create(BinaryTreeNode *_node)
{
    QueueBinaryTreeNodeNode *new_node = (QueueBinaryTreeNodeNode *)malloc(sizeof(QueueBinaryTreeNodeNode));
    new_node->_node = _node;
    new_node->_next = 0;
    return new_node;
}
void queue_binary_tree_node_node_destroy(QueueBinaryTreeNodeNode *_queue_node)
{
    free(_queue_node);
}

// declaration
typedef struct QueueBinaryTreeNode QueueBinaryTreeNode;

QueueBinaryTreeNode *queue_binary_tree_node_create();
void                 queue_binary_tree_node_push(QueueBinaryTreeNode *_queue, BinaryTreeNode *_node);
BinaryTreeNode      *queue_binary_tree_node_pop(QueueBinaryTreeNode *_queue);

int queue_binary_tree_node_is_empty(QueueBinaryTreeNode *_queue);

typedef struct QueueBinaryTreeNode
{
    QueueBinaryTreeNodeNode *_first;
    QueueBinaryTreeNodeNode  *_last;
} QueueBinaryTreeNode;

QueueBinaryTreeNode *queue_binary_tree_node_create()
{
    QueueBinaryTreeNode *queue = (QueueBinaryTreeNode *)malloc(sizeof(QueueBinaryTreeNode));
    queue->_first = 0;
    queue->_last = 0;
    return queue;
}
void queue_binary_tree_node_push(QueueBinaryTreeNode *_queue, BinaryTreeNode *_node)
{
    QueueBinaryTreeNodeNode *new_node = queue_binary_tree_node_node_create(_node);
    if(0 == _queue->_first)
    {
        _queue->_first = new_node;
    }
    else
    {
        _queue->_last->_next = new_node;
    }
    _queue->_last = new_node;
}

BinaryTreeNode *queue_binary_tree_node_pop(QueueBinaryTreeNode *_queue)
{
    if(0 == _queue->_first)
    {
        return 0;
    }
    QueueBinaryTreeNodeNode *popped_node = _queue->_first;
    BinaryTreeNode *node = popped_node->_node;
    _queue->_first = popped_node->_next;
    queue_binary_tree_node_node_destroy(popped_node);
    return node;
}

void queue_binary_tree_node_destory(QueueBinaryTreeNode *_queue)
{
    QueueBinaryTreeNodeNode *queue_node = _queue->_first;
    while(queue_node)
    {
        QueueBinaryTreeNodeNode *next_queue_node = queue_node->_next;
        queue_binary_tree_node_node_destroy(queue_node);
        queue_node = queue_node->_next;
    }
}

int queue_binary_tree_node_is_empty(QueueBinaryTreeNode *_queue)
{
    if(0 == _queue->_first)
    {
        return 1;
    }
    return 0;
}

typedef uint32_t uint;
BinaryTreeNode *binary_tree_node_create_recursive(uint _n, ...)
{
    if(0 == _n)
    {
        return 0;
    }
    va_list arg_list;
    va_start(arg_list, _n);

    BinaryTreeNode *root = binary_tree_node_create(data_create(va_arg(arg_list, char *)));
    QueueBinaryTreeNode *queue = queue_binary_tree_node_create();
    queue_binary_tree_node_push(queue, root);
    uint i = 1;
    while(!queue_binary_tree_node_is_empty(queue))
    {
        BinaryTreeNode *node = queue_binary_tree_node_pop(queue);
        // left_child:
        if(i < _n)
        {
            node->_left = binary_tree_node_create(data_create(va_arg(arg_list, char *)));
            if(node->_left)
            {
                queue_binary_tree_node_push(queue, node->_left);
            }
            i++;
        }
        else
        {
            break;
        }
        // right_child:
        if(i < _n)
        {
            node->_right = binary_tree_node_create(data_create(va_arg(arg_list, char *)));
            if(node->_right)
            {
                queue_binary_tree_node_push(queue, node->_right);
            }
            i++;
        }
        else
        {
            break;
        }
    }
    return root;
}

BinaryTreeNode *binary_tree_node_create_recursive_arr(const char *_arr[], uint _size)
{
    if(0 == _arr || 0 == _size)
    {
        return 0;
    }
    uint i = 0;
    BinaryTreeNode *root = binary_tree_node_create(data_create(_arr[i++]));
    QueueBinaryTreeNode *queue = queue_binary_tree_node_create();
    queue_binary_tree_node_push(queue, root);
    while(!queue_binary_tree_node_is_empty(queue))
    {
        BinaryTreeNode *node = queue_binary_tree_node_pop(queue);
        // left_child:
        if(i < _size)
        {
            node->_left = binary_tree_node_create(data_create(_arr[i++]));
            if(node->_left)
            {
                queue_binary_tree_node_push(queue, node->_left);
            }
        }
        else
        {
            break;
        }
        // right_child:
        if(i < _size)
        {
            node->_right = binary_tree_node_create(data_create(_arr[i++]));
            if(node->_right)
            {
                queue_binary_tree_node_push(queue, node->_right);
            }
        }
        else
        {
            break;
        }
    }
    return root;
}