#include "file_handler.h"


int FileHandler::getRiadky() { return riadky; }
int FileHandler::getStlpce() { return stlpce; }

void FileHandler::zapisHruDoSuboru(int pocetRiadkov, int pocetStlpcov, const std::vector<BUNKA>* pole, bool posledny) {
    std::ofstream subor;
    if (posledny) {
        subor.open("poslednySvet.txt");
    } else {
        subor.open("prvySvet.txt");
    }

    if (!subor.is_open()) {
        std::cout << "Subor sa nenasiel" << std::endl;
        exit(1);
    }

    subor << pocetRiadkov << std::endl;
    subor << pocetStlpcov << std::endl;
    for (int y = 1; y <= pocetRiadkov; ++y) {
        for (int x = 1; x <= pocetStlpcov; ++x) {
            const BUNKA& currentCell = (*pole)[(y - 1) * pocetStlpcov + (x - 1)];
            char znak = currentCell.ziva ? '@' : '-';
            subor << znak << " ";
        }
        subor << std::endl;
    }
    std::cout << "Zapisane" << std::endl;
    subor.close();
}

void FileHandler::nacitajHruZLokalnehoSuboru(std::string& nazovSuboru, std::vector<BUNKA>* pole) {
    std::cout << nazovSuboru << std::endl;

    std::ifstream subor(nazovSuboru);
    if (!subor.is_open()) {
        std::cout << "Subor sa nepodarilo otvorit" << std::endl;
        return;
    }

    subor >> riadky;
    subor >> stlpce;

    pole->resize(riadky*stlpce);

    char znak;
    for (int y = 1; y <= riadky; ++y) {
        for (int x = 1; x <= stlpce; ++x) {
            subor >> znak;
            BUNKA& aktualna_bunka = (*pole)[(y - 1) * stlpce + (x - 1)];
            aktualna_bunka.ziva = (znak == '@');
            aktualna_bunka.znak = znak;
        }
    }

    subor.close();
}


void FileHandler::nacitajHruZoServerSuboru(const std::string& data, std::vector<BUNKA>* pole) {
    std::istringstream iss(data);
    std::string line;

    riadky = 0, stlpce = 0;

    if (std::getline(iss, line, ';')) {
        riadky = std::stoi(line);
    }
    if (std::getline(iss, line, ';')) {
        stlpce = std::stoi(line);
    }

    pole->resize(riadky*stlpce);

    int index = 0;
    while (std::getline(iss, line, ';') && index < riadky*stlpce) {
        char znak = line[0];
        if (znak == '-') {
            (*pole)[index].ziva = false;

        } else if (znak == '@') {
            (*pole)[index].ziva = true;
        }
        (*pole)[index].znak = znak;
        //std::cout << (*pole)[index].znak << std::endl;
        index++;
    }
}