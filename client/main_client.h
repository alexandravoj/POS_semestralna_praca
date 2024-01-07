#include "../spravca/file_handler.h"
#include "../logika/game_of_life.h"
#include "../simulacia/thread_data.h"
#include "client.h"

#ifndef SEMESTRALNA_PRACA_MAIN_CLIENT_H
#define SEMESTRALNA_PRACA_MAIN_CLIENT_H

void zobrazMenu();
void zobrazMenuUloz();
int nacitajVolbu();
std::string nacitajRetazec();
int main_client(int argc, char* argv[]);

#endif //SEMESTRALNA_PRACA_MAIN_CLIENT_H
