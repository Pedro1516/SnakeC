#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>


typedef struct QueueNode
{
    void *data;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue
{
    QueueNode *head;
    QueueNode *tail;
} Queue;

Queue *create_queue();
bool enqueue(Queue *queue, void *data);
void *dequeue(Queue *queue);
void *peek_queue(Queue *queue);
void destroy_queue(Queue *queue);

#endif