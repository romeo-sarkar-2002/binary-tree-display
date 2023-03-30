#ifndef QUEUE_LAYOUT_NODE
#define QUEUE_LAYOUT_NODE

#include "layout.c"

//declarations
typedef struct QueueLayoutNode QueueLayoutNode;
QueueLayoutNode  *queue_layout_node_create(const Layout *_layout, QueueLayoutNode *_next);
void        queue_layout_node_destroy(QueueLayoutNode *_node);



typedef struct QueueLayoutNode
{
    // DoublyLinkedListNode    *_data;
    const Layout                *_layout;
   struct QueueLayoutNode        *_next;
} QueueLayoutNode;

QueueLayoutNode *queue_layout_node_create(const Layout *_layout, QueueLayoutNode *_next)
{
    QueueLayoutNode *node = (QueueLayoutNode *) malloc(sizeof(QueueLayoutNode));
    node->_layout = _layout;
    node->_next = _next;
    return node;
}

void queue_layout_node_destroy(QueueLayoutNode *_node)
{
    free(_node);
}


#endif