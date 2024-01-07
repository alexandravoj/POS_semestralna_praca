#include "../spravca/file_handler.h"
#include "../logika/game_of_life.h"
#include "../simulacia/thread_data.h"

#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <thread>
#include <regex>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <cstring>

#ifndef SEMESTRALNA_PRACA_POS_CLIENT_H
#define SEMESTRALNA_PRACA_POS_CLIENT_H



int getNacitaneRiadky();
int getNacitaneStlpce();

bool jeSpravnyFormat(const std::string& retazec);
std::string processBuffer(const char* buffer);
void vytvorBuffer(char* buffer, const char* nazovSuboru, const char* akcia);

int ulozNaServer(int sockfd, int& pocetRiadkov, int& pocetStlpcov, std::vector<BUNKA>* pole);
int stiahniZoServera(int sockfd, std::vector<BUNKA>* pole, FileHandler fh);
int client(char* hostname, int portNo, int volba, int& pocetRiadkov, int& pocetStlpcov, std::vector<BUNKA>* pole, FileHandler fh);


#endif //SEMESTRALNA_PRACA_POS_CLIENT_H
