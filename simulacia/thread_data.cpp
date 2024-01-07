
#include "thread_data.h"

void thread_data_init (std::vector<BUNKA>* pole, std::vector<BUNKA>* poleAkt, int riadky, int stlpce, ThreadData* data) {
        data->jePauza = 0;
        data->pocetZmien = 0;
        data->pole = pole;
        data->aktualny = poleAkt;
        data->riadky = riadky;
        data->stlpce = stlpce;
        data->hotovo = false;
        pthread_mutex_init(&data->mutex, NULL);
        pthread_cond_init(&data->nieJePauza, NULL);
}

void thread_data_destroy(ThreadData* data) {
    pthread_mutex_destroy(&data->mutex);
    pthread_cond_destroy(&data->nieJePauza);
}



void* naplnZasobnikDoprednou(void* data) {
    ThreadData* thread_data = (ThreadData*) data;
    int krok = 1;
    do {
        thread_data->doprednaSimulacia.push(*thread_data->pole);
        urobKrok(thread_data->riadky, thread_data->stlpce, thread_data->pole, thread_data->aktualny);
        *thread_data->pole = *thread_data->aktualny;
        vykresli(thread_data->riadky, thread_data->stlpce, thread_data->pole);
        thread_data->pocetZmien = getZmeny();
        krok++;

    } while (thread_data->pocetZmien > 0 && krok < 30);
    std::cout << "Dopredna simulacia sa naplnila do zasobnika" << std::endl;
    return NULL;
}


void* simulujSpatna(void* data) {
    ThreadData* thread_data = (ThreadData*) data;
    naplnZasobnikDoprednou(thread_data);
    int krok = 1;
    std::cout << "Zacina spatna simulacia" << std::endl;

    while (!thread_data->doprednaSimulacia.empty()) {

        std::this_thread::sleep_for(std::chrono::seconds(1));
        pthread_mutex_lock(&thread_data->mutex);
        while (thread_data->jePauza == 1) {
            pthread_cond_wait(&thread_data->nieJePauza, &thread_data->mutex);
        }
        pthread_mutex_unlock(&thread_data->mutex);

        printf("Krok cislo: %d\n", krok);

        pthread_mutex_lock(&thread_data->mutex);
        *thread_data->pole = thread_data->doprednaSimulacia.top();
        vykresli(thread_data->riadky, thread_data->stlpce, thread_data->pole);
        thread_data->doprednaSimulacia.pop();
        pthread_mutex_unlock(&thread_data->mutex);

        krok++;
    } ;

    pthread_mutex_lock(&thread_data->mutex);
    thread_data->hotovo = true;
    pthread_mutex_unlock(&thread_data->mutex);
    std::cout << "KONIEC SIMULACIE - pre potvrdenie stlacte lubovolnu klavesu\n";

    return NULL;
}


void* simulujDopredne(void* data) {
    ThreadData* thread_data = (ThreadData*) data;
    std::cout << "Zacala dopredna simulacia\n";
    int krok = 1;
    do  {

        std::this_thread::sleep_for(std::chrono::seconds(1));
        pthread_mutex_lock(&thread_data->mutex);
        while (thread_data->jePauza == 1) {
            pthread_cond_wait(&thread_data->nieJePauza, &thread_data->mutex);
        }
        pthread_mutex_unlock(&thread_data->mutex);

        printf("Krok cislo: %d\n", krok);

        pthread_mutex_lock(&thread_data->mutex);
        urobKrok(thread_data->riadky, thread_data->stlpce, thread_data->pole, thread_data->aktualny);
        thread_data->pole = thread_data->aktualny;
        vykresli(thread_data->riadky, thread_data->stlpce, thread_data->aktualny);
        thread_data->pocetZmien = getZmeny();
        pthread_mutex_unlock(&thread_data->mutex);

        krok++;

    } while (thread_data->pocetZmien > 0 && krok < 30);


    pthread_mutex_lock(&thread_data->mutex);
    thread_data->hotovo = true;
    pthread_mutex_unlock(&thread_data->mutex);
    std::cout << "KONIEC SIMULACIE - pre potvrdenie stlacte lubovolnu klavesu\n";

    return NULL;
}


//-----------------------------------------------------------------

void* spravPauzu(void* data) {
    ThreadData* thread_data = (ThreadData*) data;

    printf("Zacal pauzovac, stlacte cokolvek aby sa simulacia zastavila\n");


    while (!thread_data->hotovo){
        char buff[100];
        scanf("%s", buff);

        {
        pthread_mutex_lock(&thread_data->mutex);
        thread_data->jePauza = 1;
        pthread_mutex_unlock(&thread_data->mutex);
        }

        if (!thread_data->hotovo) {
            printf("ZASTAVENA... stlacte cokolvek pre pokracovanie v simulacii\n");
            char buff2[100];
            scanf("%s", buff2);

            {
                pthread_mutex_lock(&thread_data->mutex);
                thread_data->jePauza = 0;
                pthread_cond_signal(&thread_data->nieJePauza);
                pthread_mutex_unlock(&thread_data->mutex);
            }
        } else {
            printf("KONIEC POTVRDENY!\n");
        }

    }
    return NULL;
}
