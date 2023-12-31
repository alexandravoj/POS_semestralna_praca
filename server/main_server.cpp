#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "main_server.h"


#define CONNECTED_MAX_NO 2


int main_server(int argc, char* argv[]) {
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr, "použitie %s portu\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        perror("Error pri vytváraní socketu");
        return 1;
    }
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error pri spájaní sa s adresou socketu");
        return 2;
    }

    listen(sockfd, CONNECTED_MAX_NO);
    cli_len = sizeof(cli_addr);

    pthread_t threads[CONNECTED_MAX_NO];
    int pocetPripojenych = 0;
    SERVER_DATA serverData;

    while (1) {
        if (pocetPripojenych >= CONNECTED_MAX_NO) {
            printf("Server zaneprázdnený! Prosím, vydržte...\n");
            for (int i = 0; i < pocetPripojenych; ++i) {
                pthread_join(threads[i], NULL);

            }
            pocetPripojenych = 0;
        }
        for (int i = 0; i < pocetPripojenych; ++i) {
            pthread_join(threads[i], NULL);

        }
        int niecoOtvorene = 0;

        newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr,
                           &cli_len);

        serverData_init(&serverData, &niecoOtvorene, &newsockfd);

        pthread_create(&threads[pocetPripojenych], NULL, &serverSpracuj, &serverData);
        ++pocetPripojenych;
    }
    serverData_destroy(&serverData);
}

