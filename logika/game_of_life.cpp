#include "game_of_life.h"

int pocetZmien;

int getZmeny() {return pocetZmien;};

int spocitajSusedov(int pocetRiadkov, int pocetStlpcov, int x, int y, const std::vector<BUNKA>* pole) {
    int pocet = 0;
    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int newX = x + j;
            int newY = y + i;
            if (newX > 0 && newX <= pocetStlpcov && newY > 0 && newY <= pocetRiadkov) {
                const BUNKA& pom = (*pole)[(newY - 1) * pocetStlpcov + (newX - 1)];
                if (!(i == 0 && j == 0) && pom.ziva) {
                    pocet++;
                }
            }
        }
    }
    return pocet;
}


void znicSvet(std::vector<BUNKA>* pole) {
    if (pole != nullptr) {
        pole->clear(); // Vyprázdni vektor
        delete pole;   // Znič vektor (uvolni pamäť)
        pole = nullptr; // Nastav na nullptr, aby nedošlo k neúmyselnému opätovnému použitiu
    }
}


void vykresli(int pocetRiadkov, int pocetStlpcov, const std::vector<BUNKA>* pole) {
    std::cout << "**************************\n";
    for (int y = 0; y < pocetRiadkov; ++y) {
        for (int x = 0; x < pocetStlpcov; ++x) {
            std::cout << " |" << (*pole)[y * pocetStlpcov + x].znak << "|";
        }
        std::cout << std::endl;
    }
}


void generujSvetNahodne(int pocetRiadkov, int pocetStlpcov, std::vector<BUNKA>* pole) {
    std::cout << "******Vygenerovany svet*******" << std::endl;

    for (int y = 1; y <= pocetRiadkov; ++y) {
        for (int x = 1; x <= pocetStlpcov; ++x) {
            BUNKA& aktualna_bunka = (*pole)[(y - 1) * pocetStlpcov + (x - 1)];
            std::cout << "riadok " << y << " **** stlpec " << x;
            int randomValue = std::rand() % 2;
            aktualna_bunka.ziva = (randomValue == 1);
            aktualna_bunka.znak = aktualna_bunka.ziva ? '@' : '-';
            std::cout << " bunka znak: " << aktualna_bunka.znak << std::endl;
        }
    }
}

void vytvorSvetPouzivatel(int pocetRiadkov, int pocetStlpcov, std::vector<BUNKA>* pole) {
    for (int i = 0; i < pocetRiadkov * pocetStlpcov; ++i) {
        (*pole)[i].ziva = false;
        (*pole)[i].znak = '-';
    }
    //x - riadok, y - stlpec
    int x, y;
    char koniecZnak;
    bool zadavanie = false;
    while (!zadavanie) {
        std::cout << "Zvolte si riadok bodu (od 1 do " << pocetRiadkov << ")\n";
        std::cin >> x;
        if (x >= 1 && x <= pocetRiadkov) {
            std::cout << "Zvolte si stlpec bodu(od 1 do " << pocetStlpcov << ")\n";
            std::cin >> y;
            if (y > 0 && y <= pocetStlpcov) {
                BUNKA& aktualna_bunka = (*pole)[(y - 1) * pocetStlpcov + (x - 1)];
                aktualna_bunka.ziva = true;
                aktualna_bunka.znak = '@';
            } else {
                std::cout << "Neplatna suradnica stlpca.\n";
            }
        } else {
            std::cout << "Neplatna suradnica riadka.\n";
        }
        std::cout << "Zadajte 'q' ak chcete skoncit zadavanie suradnic alebo zadajte inu klavesu\n";
        std::cin >> koniecZnak;
        zadavanie = (koniecZnak == 'q');
    }
}


void urobKrok(int pocetRiadkov, int pocetStlpcov, const std::vector<BUNKA>* pole, std::vector<BUNKA>* pole2) {
    pocetZmien = 0;
    for (int y = 1; y <= pocetRiadkov; ++y) {
        for (int x = 1; x <= pocetStlpcov; ++x) {
            int zivychSusedov = spocitajSusedov(pocetRiadkov, pocetStlpcov, x, y, pole);
            //std::cout << "bunka: " << x << " " << y << " - " << zivychSusedov << std::endl;
            const BUNKA& currentCell = (*pole)[(y-1) * pocetStlpcov + (x-1)];
            BUNKA& newCell = (*pole2)[(y-1) * pocetStlpcov + (x-1)];
            newCell = currentCell;
            /*if ((zivychSusedov == 1 || zivychSusedov == 0) && currentCell.ziva) {
                newCell.ziva = false;
                newCell.znak = '-';
                pocetZmien++;
            } else if (zivychSusedov >= 4 && currentCell.ziva) {
                newCell.ziva = false;
                newCell.znak = '-';
                pocetZmien++;
            } else if ((zivychSusedov == 2 || zivychSusedov == 3) && currentCell.ziva) {
                newCell.ziva = true;
                newCell.znak = '@';
            } else if (zivychSusedov == 3 && !currentCell.ziva) {
                newCell.ziva = true;
                newCell.znak = '@';
                pocetZmien++;
            }*/

            if ((zivychSusedov <= 1 || zivychSusedov >= 4) && currentCell.ziva) {
                newCell.ziva = false;
                newCell.znak = '-';
                pocetZmien++;
            } else if (zivychSusedov == 3 || (currentCell.ziva && zivychSusedov == 2)) {
                if (!currentCell.ziva) {
                    pocetZmien++;
                }
                newCell.ziva = true;
                newCell.znak = '@';

            }
            //printf(" %c\n", newCell.znak);
        }
    }
}

