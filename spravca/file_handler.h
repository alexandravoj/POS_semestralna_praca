#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "../logika/game_of_life.h"

struct FileHandler {
private:
    int riadky;
    int stlpce;
public:
    FileHandler() : riadky(0), stlpce(0) {};
    ~FileHandler() {riadky = 0, stlpce = 0;};
    int getRiadky();
    int getStlpce();
    void zapisHruDoSuboru(int pocetRiadkov, int pocetStlpcov, const std::vector<BUNKA>* pole, bool posledny);
    void nacitajHruZLokalnehoSuboru(std::string& nazovSuboru, std::vector<BUNKA>* pole);
    void nacitajHruZoServerSuboru(const std::string& data, std::vector<BUNKA>* pole);

};




