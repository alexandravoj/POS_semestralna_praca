//
// Created by ivkao on 3. 1. 2024.
//

#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <pthread.h>
#include <unistd.h>

void server_save_load_init(SERVER_DATA* data, int* niecoOtvorene, int* newsockfd) {
    data->niecoOtvorene = niecoOtvorene;
    data->newsockfd = newsockfd;
    data->mutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));
    data->isNotOpened = (pthread_cond_t*)malloc(sizeof(pthread_cond_t));
    pthread_mutex_init(data->mutex, NULL);
    pthread_cond_init(data->isNotOpened, NULL);
};

void server_save_load_destroy(SERVER_DATA* data) {
    free(data->niecoOtvorene);
    data->niecoOtvorene = NULL;
    free(data->newsockfd);
    data->newsockfd = NULL;
    pthread_mutex_destroy(data->mutex);
    pthread_cond_destroy(data->isNotOpened);
    free(data->mutex);
    data->mutex = NULL;
    free(data->isNotOpened);
    data->isNotOpened = NULL;
};

void* serverSaveLoadThread(void* data) {
    SERVER_DATA* s_data = (SERVER_DATA*)data;
    int newsockfd;
    pthread_mutex_lock(s_data->mutex);
    newsockfd = *s_data->newsockfd;
    pthread_mutex_unlock(s_data->mutex);

    int n;
    char buffer[1000];

    if (newsockfd < 0) {
        perror("ERROR pri potvrdení");
        close(newsockfd);
        printf("Klient odpojený zo socketu %d\n", newsockfd);
        return NULL;
    }

    printf("Klient pripojený k socketu %d\n", newsockfd);

    bzero(buffer, 1000);
    n = read(newsockfd, buffer, 999);
    if (n < 0) {
        perror("Error pri načítaní zo socketu");
        close(newsockfd);
        printf("Klient odpojený zo socketu %d\n", newsockfd);
        return NULL;
    }
    printf("Prečítal %d\n", newsockfd);
    char filename[100];


    int i = 5;
    while (buffer[i] != '\n' && buffer[i] != '\0' && i < 100) {
        filename[i-5] = buffer[i];
        ++i;
    }
    filename[i-5] = '\0';
    printf("Názov súboru: '%s' '%s'\n", filename, buffer[0] == 'u' ? "uloz" : "nacitaj");
    printf("Socket %d: \n", newsockfd);


    pthread_mutex_lock(s_data->mutex);
    while (*s_data->niecoOtvorene == 1) {
        pthread_cond_wait(s_data->isNotOpened, s_data->mutex);
    }
    *s_data->niecoOtvorene = 1;
    pthread_mutex_unlock(s_data->mutex);

    if (buffer[0] == 'u') {

        FILE* out = fopen(filename, "w");
        fprintf(out, "%s", buffer);
        fclose(out);
        bzero(buffer, 1000);
        const char* msg = "Súbor prijatý";
        n = write(newsockfd, msg, strlen(msg)+1);

    } else if (buffer[0] == 'n'){
        FILE* in = fopen(filename, "r");
        bzero(buffer, 1000);

        if (!in) {
            printf("Error: súbor sa nenašiel\n");
            const char* msg = "Neexistuje subor s tymto nazvom";
            n = write(newsockfd, msg, strlen(msg)+1);
            close(newsockfd);
            printf("Klient odpojený zo socketu %d\n", newsockfd);
            return NULL;
        }

        int j = 0;
        while (!feof(in)) {
            char c = getc(in);
            buffer[j] = c;
            ++j;
        }
        buffer[j] = '\0';

        n = write(newsockfd, buffer, 999);
        if (n < 0) {
            perror("Error pri zapísaní na socket");
            pthread_mutex_lock(s_data->mutex);
            *s_data->niecoOtvorene = 0;
            pthread_cond_signal(s_data->isNotOpened);
            pthread_mutex_unlock(s_data->mutex);
            close(newsockfd);
            printf("Klient odpojený zo socketu %d\n", newsockfd);
            return NULL;
        }

        fclose(in);
    }

    pthread_mutex_lock(s_data->mutex);
    *s_data->niecoOtvorene = 0;
    pthread_cond_signal(s_data->isNotOpened);
    pthread_mutex_unlock(s_data->mutex);

    close(newsockfd);
    printf("Klient odpojený zo socketu %d\n", newsockfd);

    /*char koniec[10]; // Inicializujte reťazec s dostatočným priestorom pre vstup
    printf("Stlačte ´q´ ak chcete ukoncit pracu servera.\n");
    scanf("%9s", koniec); // Načítajte maximálne 9 znakov, aby ste predišli pretečeniu buffera
    if(strcmp(koniec, "q") == 0) { // Porovnajte reťazce pomocou strcmp
        exit(0);
    }*/

    return NULL;
}

