//
// Created by ivkao on 3. 1. 2024.
//

#ifndef SERVER_C_SERVER_H
#define SERVER_C_SERVER_H
#include <pthread.h>

typedef struct serverData {
    int* niecoOtvorene;
    int* newsockfd;
    pthread_mutex_t* mutex;
    pthread_cond_t* isNotOpened;
} SERVER_DATA;


void serverData_init(SERVER_DATA* data, int* niecoOtvorene, int* newsockfd);
void serverData_destroy(SERVER_DATA* data);

void* serverSpracuj(void* data);

#endif //SERVER_C_SERVER_H
