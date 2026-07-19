#include "../includes/queue.h"
#include <stdlib.h>

Queue *create_queue()
{
    Queue *queue = malloc(sizeof(Queue));
    if (queue == NULL)
        return NULL;
        
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

bool enqueue(Queue *queue, void *data)
{
    if (queue == NULL)
        return false;

    QueueNode *node = malloc(sizeof(QueueNode));
    if (node == NULL)
        return false;

    node->data = data;
    node->next = NULL;

    if (queue->head == NULL)
    {
        queue->head = node;
        queue->tail = node;
        return true;
    }

    queue->tail->next = node;
    queue->tail = node;

    return true;
}

void *peek_queue(Queue *queue)
{
    if (queue == NULL || queue->head == NULL)
        return NULL;

    return queue->head->data;
}

void *dequeue(Queue *queue)
{
    if (queue == NULL || queue->head == NULL)
        return NULL;

    QueueNode *node = queue->head;
    void *data = node->data;

    queue->head = node->next;
    free(node);

    if (queue->head == NULL)
        queue->tail = NULL;

    return data;
}