
#include "../spravca/file_handler.h"
#include "../logika/game_of_life.h"
#include "../simulacia/thread_data.h"
#include "client.h"
#include "main_client.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <thread>
#include <cstring>



void zobrazMenu() {
    std::cout << "------GAME OF LIFE----\n";
    std::cout << "1. generuj svet\n";
    std::cout << "2. vytvor si vlastny svet\n";
    std::cout << "3. nacitaj zo suboru\n";
    std::cout << "4. stiahni vzor sveta zo servera\n";
    std::cout << "5. uloz vzor sveta na server\n";
    std::cout << "Vyberte moznost: ";
}

void zobrazMenuUloz() {
    std::cout << "**************************\n";
    std::cout << "Chcete ulozit svet do lokalneho suboru?\n";
    std::cout << "Chcem ulozit svet - 1\n";
    std::cout << "Nechcem ulozit svet - cokolvek\n";
}

int nacitajVolbu() {
    int volba;
    std::cin >> volba;
    return volba;
}

std::string nacitajRetazec() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string retazec;
    std::cout << "Zadajte retazec: ";
    std::getline(std::cin, retazec);
    return retazec;
}

int main_client(int argc, char* argv[]) {
    FileHandler fh;


    srand(time(NULL));
    int pocetRiadkov = 0;
    int pocetStlpcov = 0;
    int volba;

    zobrazMenu();
    volba = nacitajVolbu();

    if (volba == 5) {
        std::cout << "Ak chcete ulozit vzor na server, musite si ho najskor vytvorit\n";
    }

    if (volba == 1 || volba == 2 || volba == 5) {
        std::cout << "Zadaj pocet riadkov: \n";
        pocetRiadkov = nacitajVolbu();
        std::cout << "Zadaj pocet stlpcov: \n";
        pocetStlpcov = nacitajVolbu();
        /*if (pocetRiadkov == 1 && pocetStlpcov == 1) {
            std::cout << "Svet musí byt vacsi ako 1x1\n";
            return 1;
        }*/
    }

    std::vector<BUNKA>* pole = new std::vector<BUNKA>(pocetRiadkov * pocetStlpcov);

    std::cout << "******************************\n";
    if (volba == 1) {
        generujSvetNahodne(pocetRiadkov, pocetStlpcov, pole);
    } else if (volba == 2) {
        vytvorSvetPouzivatel(pocetRiadkov, pocetStlpcov, pole);
    } else if (volba == 3) {
        std::string vstup = nacitajRetazec();
        fh.nacitajHruZLokalnehoSuboru(vstup, pole);
        pocetRiadkov = fh.getRiadky();
        pocetStlpcov = fh.getStlpce();
    } else if (volba == 4) {
        client(argv[1], atoi(argv[2]), volba, pocetRiadkov, pocetStlpcov, pole, fh);
    } else if (volba == 5) {
        pole = new std::vector<BUNKA>(pocetRiadkov * pocetStlpcov);
        client(argv[1], atoi(argv[2]), volba, pocetRiadkov, pocetStlpcov, pole, fh);
    } else {
        std::cout << "neexistujuca moznost\n";
        return 1;
    }

    if (volba == 4 || volba == 5) {
        pocetRiadkov = getNacitaneRiadky();
        pocetStlpcov = getNacitaneStlpce();
    }

    if (volba != 5) {
        vykresli(pocetRiadkov, pocetStlpcov, pole);
    }


    //SIMULACIA

    if (pole->size() != 0 ) {
        std::cout << "Chcete spustit simulaciu? ano - 1 / nie - cokolvek\n";
        volba = nacitajVolbu();

        if (volba == 1) {
            std::vector<BUNKA>* poleAkt = new std::vector<BUNKA>(pocetRiadkov * pocetStlpcov);
            std::cout << "Ak chcete doprednu - 0 / spätnu - 1\n";
            volba = nacitajVolbu();
            ThreadData threadData;
            thread_data_init(pole, poleAkt, pocetRiadkov, pocetStlpcov, &threadData);
            pthread_t threadPauzovac, threadSimulacia;
            if (volba == 0) {
                pthread_create(&threadSimulacia, NULL, simulujDopredne, &threadData);
                pthread_create(&threadPauzovac, NULL, spravPauzu, &threadData);
                pthread_join(threadSimulacia, NULL);
                pthread_join(threadPauzovac, NULL);

            } else if (volba == 1) {
                pthread_create(&threadSimulacia, NULL, simulujSpatna, &threadData);
                pthread_create(&threadPauzovac, NULL, spravPauzu, &threadData);
                pthread_join(threadSimulacia, NULL);
                pthread_join(threadPauzovac, NULL);
            } else {
                perror("nespravne zadane");
                znicSvet(poleAkt);
                znicSvet(pole);
                return 2;
            }

            thread_data_destroy(&threadData);
            std::cout << "******************************\n";
            std::cout << "Simulacia skoncila, -> posledny svet\n";
            vykresli(pocetRiadkov, pocetStlpcov, poleAkt);

            //UKLADANIE LOKALNYCH SUBOROV

            zobrazMenuUloz();
            volba = nacitajVolbu();
            if (volba == 1) {
                fh.zapisHruDoSuboru(pocetRiadkov, pocetStlpcov, poleAkt, true);
            }else {
                std::cout << "Nezapisali ste nic\n";
            }
            znicSvet(poleAkt);

        }

        //UKLADANIE LOKALNYCH SUBOROV

        std::cout << " - > prvy svet\n";
        vykresli(pocetRiadkov, pocetStlpcov, pole);

        std::cin.clear();
        zobrazMenuUloz();
        volba = nacitajVolbu();

        if (volba == 1) {
            fh.zapisHruDoSuboru(pocetRiadkov, pocetStlpcov, pole, false);
        }  else {
            std::cout << "Nezapisali ste nic\n";
        }
    }


    znicSvet(pole);

    std::cout << "KONIEC GAME OF LIFE\n";

    return 0;
}
