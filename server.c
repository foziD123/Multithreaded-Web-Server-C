#include "segel.h"
#include "request.h"
#include "queue.h"

#define MAX_WORD 7

pthread_mutex_t m;
pthread_cond_t not_empty;   // condition variable for queue not empty
pthread_cond_t not_full;    // condition variable for queue not full

Queue wait_queue = NULL;
Queue current_requests_queue = NULL;
bool IsOverLoad(Queue wait_queue,Queue current_requests_queue, int max_requests)
{
    return QueueSize(wait_queue) + QueueSize(current_requests_queue) >= max_requests;
}
void CreateThreads(pthread_t *thread_pool,int thread[],int max_threads ,void* thread_routine(void *thread) )
{
    for (int i = 0; i < max_threads; i++) {
        thread[i] = i;                                  // Assign values to the elements of the thread array
        pthread_create(&thread_pool[i], NULL, thread_routine, (void *) &thread[i]);
    }
}

void *thread_routine(void *thread) {                         // thread routine which is called by pthread_create

    int static_requests = 0;
    int dynamic_requests = 0;
    int total_request = 0;

    while (true) {                                           // infinite loop to handle requests from queue
        pthread_mutex_lock(&m);                              // lock mutex to access queue
        while (IsQueueEmpty(wait_queue)) {
            pthread_cond_wait(&not_empty, &m);              //thread is blocked if the wait queue is empty
        }
        Time arrival;
        arrival = GetTIme(wait_queue);
        int request_fd = Pop(wait_queue);
        if (request_fd == -1) {                             // if the request is -1, it means that the queue is empty
            pthread_mutex_unlock(&m);
            continue;
        }

        Push(current_requests_queue, request_fd, arrival);  // push the request to current request queue
        pthread_mutex_unlock(&m);

        Time dispatch;
        gettimeofday(&dispatch, NULL);
        requestHandle(request_fd, &total_request, &dynamic_requests,
                      &static_requests, arrival, dispatch, *(int *) thread);

        Close(request_fd);
        pthread_mutex_lock(&m);                         // lock mutex to access current queue

        PopByIndex(current_requests_queue, FindReq(current_requests_queue, request_fd));

        pthread_cond_signal(&not_full);                 // signal that the queue is not full
        pthread_mutex_unlock(&m);
    }
    return NULL;
}


// HW3: Parse the new arguments too
void getargs(int *port, int argc, char *argv[], int *max_threads, int *max_requests, char *policy) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    strcpy(policy, argv[4]);
    *max_requests = atoi(argv[3]);
    *max_threads = atoi(argv[2]);
    *port = atoi(argv[1]);
}


int main(int argc, char *argv[]) {

    wait_queue = InitQueue();
    current_requests_queue = InitQueue();

    pthread_mutex_init(&m, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    int listenfd, connfd, port, clientlen;
    int max_threads, max_requests;
    struct sockaddr_in clientaddr;
    char policy[MAX_WORD];

    getargs(&port, argc, argv, &max_threads, &max_requests, policy);
    int thread[max_threads];
    pthread_t *thread_pool = malloc(sizeof(thread_pool) * max_threads);
    CreateThreads(thread_pool,thread,max_threads,thread_routine);
    listenfd = Open_listenfd(port);
    Time curr_time;
    srand(time(NULL)); // init random seed
    while (true) {
        clientlen = sizeof(clientaddr);
        connfd = Accept(listenfd, (SA *) &clientaddr, (socklen_t *) &clientlen);

        gettimeofday(&curr_time, NULL);

        pthread_mutex_lock(&m);
        if (IsOverLoad(wait_queue,current_requests_queue,max_requests)) {
            if (strcmp("dt", policy) == 0) {
                Close(connfd);
                pthread_mutex_unlock(&m);
                continue;
            }
            if (strcmp("dh", policy) == 0) {
                int connfd_head = Pop(wait_queue);
                if (connfd_head == -1) {
                    Close(connfd);
                    pthread_mutex_unlock(&m);
                    continue;
                }
                Close(connfd_head);
            }
            if (strcmp("block", policy) == 0) {
                while (QueueSize(wait_queue) + QueueSize(current_requests_queue) >= max_requests) {
                    pthread_cond_wait(&not_full, &m);
                }
            }
            if (strcmp("random", policy) == 0) {
                int size = QueueSize(wait_queue);
                int dropsize = size * 0.5;
                if ((size * 5) % 10 != 0) {
                    dropsize++;
                }

                if (dropsize == 0) {
                    Close(connfd);
                    pthread_mutex_unlock(&m);
                    continue;
                }
                for (int i = 0; i < dropsize; ++i) {
                    if (size == 0) {
                        break;
                    }
                    int rand_index = rand() % size;
                    int deleting_fd = PopByIndex(wait_queue, rand_index);
                    Close(deleting_fd);
                    size--;
                }
                if (QueueSize(wait_queue) + QueueSize(current_requests_queue) >= max_requests) {
                    Close(connfd);
                    pthread_mutex_unlock(&m);
                    continue;
                }
            }
        }
        Push(wait_queue, connfd, curr_time);
        pthread_cond_signal(&not_empty);
        pthread_mutex_unlock(&m);
    }
}
