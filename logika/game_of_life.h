#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <cstdlib>
#include "../logika/bunka.h"


void znicSvet(std::vector<BUNKA>* pole);
int spocitajSusedov(int pocetRiadkov, int pocetStlpcov, int x, int y, const std::vector<BUNKA>* pole);
void vykresli(int pocetRiadkov, int pocetStlpcov,const std::vector<BUNKA>* pole);
int getZmeny();

void generujSvetNahodne(int pocetRiadkov, int pocetStlpcov, std::vector<BUNKA>* pole);
void vytvorSvetPouzivatel(int pocetRiadkov, int pocetStlpcov, std::vector<BUNKA>* pole);
void urobKrok(int pocetRiadkov, int pocetStlpcov, const std::vector<BUNKA>* pole, std::vector<BUNKA>* pole2);

