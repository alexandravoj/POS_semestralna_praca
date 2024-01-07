
#include "../logika/game_of_life.h"

#include <mutex>
#include <queue>
#include <random>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <thread>
#include <regex>
#include <stack>
#include <pthread.h>
#include <condition_variable>
#include <stdio.h>



#ifndef CLIENT_CPP_THREADDATA_H
#define CLIENT_CPP_THREADDATA_H


struct ThreadData {
    std::vector<BUNKA>* aktualny;
    std::vector<BUNKA>* pole;
    int jePauza;
    int pocetZmien;
    int riadky;
    int stlpce;
    bool hotovo;
    std::stack<std::vector<BUNKA>> doprednaSimulacia;
    pthread_mutex_t mutex;
    pthread_cond_t nieJePauza;

};

void thread_data_init (std::vector<BUNKA>* pole, std::vector<BUNKA>* poleAkt, int riadky, int stlpce, ThreadData* data);

void thread_data_destroy(ThreadData* data);

void* simulujDopredne(void* data);

void* naplnZasobnikDoprednou(void* data);

void* simulujSpatna(void* data);

void* spravPauzu(void* data);


#endif //CLIENT_CPP_THREADDATA_H*/
