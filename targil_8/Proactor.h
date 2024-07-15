#ifndef PROACTOR_H
#define PROACTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef void* (*proactorFunc)(int fd);

typedef struct {
    int sockfd;
    proactorFunc handler;
} ProactorArgs;

pthread_t startProactor(int sockfd, proactorFunc handler);
int stopProactor(pthread_t tid);
void* proactorThread(void* args);

#endif // PROACTOR_H
