#include "client.h"
#include <string>
#include <vector>
#include <iostream>
#include <regex>
#include <cstring>

int riadky;
int stlpce;

int getNacitaneRiadky() {return riadky;};
int getNacitaneStlpce() {return stlpce;};


bool jeSpravnyFormat(const std::string& retazec) {
    std::regex vzor("^[A-Za-z0-9]+\\.txt$");
    return std::regex_match(retazec, vzor);
}

std::string processBuffer(const char* buffer) {
    std::string nacitany;
    int riadok = 0;
    int index = 0;
    while (riadok < 2) {
        if (buffer[index] == '\n') {
            riadok++;
        }
        index++;
    }
    while (buffer[index] != '\n') {
        nacitany += buffer[index];
        index++;
    }

    return nacitany;
}

void vytvorBuffer(char* buffer, const char* nazovSuboru, const char* akcia) {
    bzero(buffer, 1000);
    strcat(buffer, akcia);
    strcat(buffer, nazovSuboru);
    strcat(buffer, "\n");
}


int ulozNaServer(int sockfd, int& pocetRiadkov, int& pocetStlpcov, std::vector<BUNKA>* pole) {
    int n;
    char buffer[1000];
    riadky = pocetRiadkov;
    stlpce = pocetStlpcov;
    vytvorSvetPouzivatel(pocetRiadkov, pocetStlpcov, pole);
    printf("Zadaj meno suboru (format nazov.txt):");
    char str[20];
    scanf("%s", str);
    if (!jeSpravnyFormat(str)) {
        printf("Nespravny format\n");
    } else {
    vytvorBuffer(buffer, str, "uloz\n");
    sprintf(str, "%d;", pocetRiadkov);
    strcat(buffer, str);
    sprintf(str, "%d;", pocetStlpcov);
    strcat(buffer, str);
    printf("%s\n", buffer);
    for (int y = 1; y <= pocetRiadkov; ++y) {
        for (int x = 1; x <= pocetStlpcov; ++x) {
            char temp[3] = {(*pole)[(y - 1) * pocetStlpcov + (x - 1)].znak,';','\0'};
            strcat(buffer, temp);
        }

    }
    strcat(buffer, "\n");
    printf("%s\n", buffer);

    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        perror("Error wpri zapisovani na socket");
        return 5;
    }

    printf("******Data poslane*******\n");

    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return 6;
    }

    printf("%s\n",buffer);
    }

}


int stiahniZoServera(int sockfd, std::vector<BUNKA>* pole, FileHandler fh) {
    int n;
    char buffer[1000];
    printf("Zadaj meno suboru (format nazov.txt):");
    char str[20];
    scanf("%s", str);
    if (!jeSpravnyFormat(str)) {
        printf("Nespravny format\n");
    } else {
        vytvorBuffer(buffer, str, "nctj\n");
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error pri zapisovani na socket");
            return 5;
        }

        bzero(buffer, 1000);
        n = read(sockfd, buffer, 999);
        if (n < 0) {
            perror("Error pri citani zo socketu");
            return 6;
        }

        if (strcmp(buffer, "Neexistuje subor s tymto nazvom") == 0) {
            printf("%s\n",buffer);
            return 7;
        }

        std::string data = processBuffer(buffer);
        std::cout << data << std::endl;
        fh.nacitajHruZoServerSuboru(data, pole);
        riadky = fh.getRiadky();
        stlpce = fh.getStlpce();
        std::cout << "***************" << std::endl;
        vykresli(riadky, stlpce, pole);

        printf("****Vzor sa nacital*****\n");
    }
}


int client(char* hostname, int portNo, int volba, int& pocetRiadkov, int& pocetStlpcov, std::vector<BUNKA>* pole, FileHandler fh) {
    int sockfd;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr, "Error, host neexistuje\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(portNo);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error pri vytváraní socketu");
        return 3;
    }

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error pri pripájaní k socketu");
        return 4;
    }

    if (volba == 5) {
        ulozNaServer(sockfd, pocetRiadkov, pocetStlpcov, pole);
    } else {
        stiahniZoServera(sockfd, pole, fh);
    }

    close(sockfd);
    return 0;
}




