#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "Proactor.h"

typedef void* (*proactorFunc)(int fd);

// typedef struct {
//     int sockfd;
//     proactorFunc handler;
// } ProactorArgs;

// pthread_t startProactor(int sockfd, proactorFunc handler);
// int stopProactor(pthread_t tid);
// void* proactorThread(void* args);

pthread_t startProactor(int sockfd, proactorFunc handler) {
    pthread_t tid;
    ProactorArgs* args = (ProactorArgs*)malloc(sizeof(ProactorArgs));
    if (!args) {
        perror("malloc");
        return 0;
    }
    args->sockfd = sockfd;
    args->handler = handler;

    if (pthread_create(&tid, NULL, proactorThread, (void*)args) != 0) {
        perror("pthread_create");
        free(args); // Free memory in case of error
        return 0;
    }
    return tid;
}

int stopProactor(pthread_t tid) {
    if (pthread_cancel(tid) != 0) {
        perror("pthread_cancel");
        return -1;
    }
    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        return -1;
    }
    return 0;
}

void* proactorThread(void* args) {
    ProactorArgs* pargs = (ProactorArgs*)args;
    pargs->handler(pargs->sockfd);
    free(pargs); // Free allocated memory here
    return NULL;
}
