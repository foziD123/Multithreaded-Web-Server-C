#ifndef HW3_OS_QUEUE_H
#define HW3_OS_QUEUE_H

#include <stdbool.h>
#include <sys/time.h>
#define ERROR -1
#define NOTFOUND 0
typedef enum Res {
    SUCCESS,
    FAILURE
} Res;

typedef struct queue_t *Queue;
typedef struct node_t *Node;
typedef struct timeval Time;

Queue InitQueue();

Res Push(Queue Queue, int request_id, Time arrival_time);

int Pop(Queue Queue);

int QueueSize(Queue Queue);

bool IsQueueFull(Queue Queue);

bool IsQueueEmpty(Queue Queue);

int FindReq(Queue Queue, int request_id);

int PopByIndex(Queue Queue, int index);

void DestroyQueue(Queue Queue);

Time GetTIme(Queue Queue1);



#endif //HW3_OS_QUEUE_H
