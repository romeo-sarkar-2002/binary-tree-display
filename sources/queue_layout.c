#ifndef QUEUE_LAYOUT
#define QUEUE_LAYOUT


#include <stdio.h>
#include <stdlib.h>


#include "queue_layout_node.c"


// declarations
typedef struct QueueLayout QueueLayout;

QueueLayout    *queue_layout_create();
void            queue_layout_push(QueueLayout *_queue, const Layout *_layout);
const Layout   *queue_layout_pop(QueueLayout *_queue);
void            queue_layout_destory(QueueLayout *_queue);
int             queue_layout_is_empty(QueueLayout *_queue);

typedef struct QueueLayout
{
    QueueLayoutNode    *_head;
    QueueLayoutNode    *_tail;
    QueueLayoutNode   *_first;
} QueueLayout;

QueueLayout *queue_layout_create()
{
    QueueLayout *queue = malloc(sizeof(QueueLayout));
    ERROR(queue_layout_create, 0 == queue, "malloc returned (null)", return 0);
    queue->_head = queue->_tail = queue->_first = 0;
    return queue;
}

void queue_layout_push(QueueLayout *_queue, const Layout *_layout)
{
    ERROR(queue_layout_push, 0 == _queue, "_queue is (null)", return);
    QueueLayoutNode *new_node = queue_layout_node_create(_layout, 0);
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
    QueueLayoutNode *head = _queue->_head;
    while(head)
    {
        QueueLayoutNode *removed_node = head;
        head = removed_node->_next;
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
    QueueLayoutNode *queue_layout_node = _queue->_first;
    printf("\e[1;31m~[\e[m");
    while(queue_layout_node)
    {
        printf("\e[1;33m%s\e[m", queue_layout_node->_layout->_str);
        queue_layout_node = queue_layout_node->_next;
        if(queue_layout_node)
        {
            printf("\e[1;31m|\e[m");
        }
    }
    printf("\e[1;31m]\e[m\n");
}

QueueLayoutNode *queue_layout_head(QueueLayout *_queue)
{
    return _queue->_head;
}
#endif 