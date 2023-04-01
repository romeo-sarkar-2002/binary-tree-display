#ifndef QUEUE_LAYOUT
#define QUEUE_LAYOUT


#include <stdio.h>
#include <stdlib.h>

#include "layout_internal.c"

//declarations
typedef struct QueueLayoutNode QueueLayoutNode;

QueueLayoutNode    *queue_layout_node_create(const Layout *_layout);
void                queue_layout_node_destroy(QueueLayoutNode *_node);


typedef struct QueueLayoutNode
{
    const Layout          *_layout;
   struct QueueLayoutNode   *_next;
} QueueLayoutNode;

QueueLayoutNode *queue_layout_node_create(const Layout *_layout)
{
    QueueLayoutNode *new_node = (QueueLayoutNode *) malloc(sizeof(QueueLayoutNode));
    new_node->_layout = _layout;
    new_node->_next = 0;
    return new_node;
}

void queue_layout_node_destroy(QueueLayoutNode *_queue_node)
{
    free(_queue_node);
}


// declarations
typedef struct QueueLayout QueueLayout;

QueueLayout        *queue_layout_create();
void                queue_layout_destroy(QueueLayout *_queue);

void                queue_layout_push(QueueLayout *_queue, const Layout *_layout);
const Layout       *queue_layout_pop(QueueLayout *_queue);
void                queue_layout_print(QueueLayout *_queue);

int                 queue_layout_is_empty(QueueLayout *_queue);
QueueLayoutNode    *queue_layout_get_head(QueueLayout *_queue);



typedef struct QueueLayout
{
    QueueLayoutNode    *_head;
    QueueLayoutNode    *_tail;
    QueueLayoutNode   *_first;
} QueueLayout;

QueueLayout *queue_layout_create()
{
    QueueLayout *queue_layout = (QueueLayout *)malloc(sizeof(QueueLayout));
    ERROR(queue_layout_create, 0 == queue_layout, "malloc returned (null)", return 0);

    queue_layout->_head = 0;
    queue_layout->_tail = 0;
    queue_layout->_first = 0;
    return queue_layout;
}

void queue_layout_push(QueueLayout *_queue, const Layout *_layout)
{
    ERROR(queue_layout_push, 0 == _queue, "_queue is (null)", return);

    QueueLayoutNode *new_node = queue_layout_node_create(_layout);
    if(0 == _queue->_head)
    {
        _queue->_head = new_node;
    }

    if(0 == _queue->_first)
    {
        _queue->_first = new_node;
    }
    if(_queue->_tail)
    {
        _queue->_tail->_next = new_node;
    }
    _queue->_tail = new_node;
}

const Layout *queue_layout_pop(QueueLayout *_queue)
{
    ERROR(queue_layout_pop, 0 == _queue->_first, "_queue is empty", return 0);

    QueueLayoutNode *popped_node = _queue->_first;
    const Layout *layout = popped_node->_layout;
    _queue->_first = popped_node->_next;
    return layout;
}

void queue_layout_destroy(QueueLayout *_queue)
{
    QueueLayoutNode *queue_head = _queue->_head;
    while(queue_head)
    {
        QueueLayoutNode *removed_node = queue_head;
        queue_head = removed_node->_next;
        queue_layout_node_destroy(removed_node);
    }
    free(_queue);
}

int queue_layout_is_empty(QueueLayout *_queue)
{
    if(0 == _queue->_first)
    {
        return 1;
    }
    return 0;
}

void queue_layout_print(QueueLayout *_queue)
{
    QueueLayoutNode *queue_node = _queue->_first;
    printf("\e[1;31m~[\e[m");
    while(queue_node)
    {
        printf("\e[1;33m%s\e[m", queue_node->_layout->_str);
        queue_node = queue_node->_next;
        if(queue_node)
        {
            printf("\e[1;31m|\e[m");
        }
    }
    printf("\e[1;31m]\e[m");
}

QueueLayoutNode *queue_layout_get_head(QueueLayout *_queue)
{
    return _queue->_head;
}
#endif 