#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

struct node_t {
    int request_id;     // connection file descriptor
    Time arrival_time;
    Time dipach_time;
    struct node_t *next;
};

struct queue_t {
    int size;
    Node head;
    Node tail;
};

int PopByReqfd(Queue queue, int to_delete) {                // delete node by request id
    if (!queue ) {
        return ERROR;
    }
    if(to_delete < 0)
    {
        return ERROR;
    }
    if( queue->size == 0)
    {
        return ERROR;
    }

    Node iter = queue->head;
    if (iter != NULL && iter->request_id == to_delete) {
        queue->head = iter->next;
        free(iter);
        queue->size--;
        return ERROR;
    }

    while (iter->next != NULL) {
        if (iter->next->request_id == to_delete) {
            break;
        }
        iter = iter->next;
    }
    if (iter->next == NULL) {
        //didnt find node to delete,hence return
        return NOTFOUND;
    }
    Node delete = iter->next;
    iter->next = delete->next;
    free(delete);
    queue->size--;
    return to_delete;
}


int Pop(Queue Queue) { //pop from the head of the queue, oldest request

    if (!Queue ) {
        //we have nothing to pop because the queue is empty
        return ERROR;
    }
    if( Queue->size == 0)
    {
        return ERROR;
    }
    int curr_connfd = Queue->head->request_id;
    PopByReqfd(Queue, curr_connfd);
    return curr_connfd;
}

Queue InitQueue() {
    Queue Queue = malloc(sizeof(*Queue));
    if (!Queue) {
        exit(1);
    }
    Queue->size = 0;
    Queue->head = NULL;
    Queue->tail = NULL;
    return Queue;
}


Res Push(Queue Queue, int request_id, Time arrival_time) {

    if (!Queue ) {
        return SUCCESS;
    }

    Node new_node = malloc(sizeof(*new_node));
    if (!new_node ) {
        exit(1);
    }

    Node iter = Queue->head;
    if (!iter) { //if the queue is empty we add the new node to the head
        Queue->head = new_node;
        Queue->head->request_id = request_id;
        Queue->head->arrival_time = arrival_time;
        Queue->head->next = NULL;
        Queue->size++;
        return SUCCESS;
    }
    while (iter->next ) {       //if the queue is not empty we add the new node to the tail
        iter = iter->next;
    }
    iter->next = new_node;
    Queue->size++;
    iter->next->request_id = request_id;
    iter->next->arrival_time = arrival_time;
    iter->next->next = NULL;

    return SUCCESS;
}

int QueueSize(Queue Queue) {
    return Queue->size;
}

bool IsQueueEmpty(Queue Queue) {
    return Queue->size == 0;
}

int FindReq(Queue Queue, int request_id) {  //find the index of the node with the given request id
    int index = 0;
    Node tmp = Queue->head;
    while (tmp) {
        if (tmp->request_id == request_id) {
            return index;
        }
        tmp = tmp->next;
        index++;
    }
    return ERROR;
}

int PopByIndex(Queue Queue, int index) {    //pop node by index using PopByReqfd
    {
        if (Queue == NULL) {
            return ERROR;
        }

        Node iter = Queue->head;
        for (int i = 0; i < index; i++) {
            iter = iter->next;
        }
        int connfd = iter->request_id;
        PopByReqfd(Queue, iter->request_id);
        return connfd;
    }
}

void DestroyQueue(Queue Queue) {
    if (!Queue) {
        return;
    }
    while (!IsQueueEmpty(Queue)) {
        Node tmp = Queue->head;
        Queue->head = Queue->head->next;
        free(tmp);
    }
    free(Queue);
}

Time GetTIme(Queue Queue1) {    //get the time of the head of the queue
    return Queue1->head->arrival_time;
}

