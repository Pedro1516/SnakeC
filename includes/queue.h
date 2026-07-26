#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>
#include "types.h"

typedef struct QueueNode
{
    void *data;
    struct QueueNode *next;
} QueueNode;

struct Queue
{
    QueueNode *head;
    QueueNode *tail;
};

Queue *create_queue();
bool enqueue(Queue *queue, void *data);
void *dequeue(Queue *queue);
void *peek_queue(Queue *queue);
void destroy_queue(Queue *queue);

#endif