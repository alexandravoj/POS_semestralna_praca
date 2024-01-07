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


void server_save_load_init(SERVER_DATA* data, int* niecoOtvorene, int* newsockfd);
void server_save_load_destroy(SERVER_DATA* data);

void* serverSaveLoadThread(void* data);

#endif //SERVER_C_SERVER_H
