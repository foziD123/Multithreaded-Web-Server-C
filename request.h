#ifndef __REQUEST_H__
#include "stdbool.h"
void PrintStats(char buf[MAXLINE] ,struct timeval arrival_time ,struct timeval diff ,int thread ,int total_requests ,int static_requests ,int dynamic_requests, bool dyn);
void requestHandle(int fd,int * total_requests, int * dynamic_requests, int * static_requests, struct timeval arrival, struct timeval dispatch, int thread);

#endif
