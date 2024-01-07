#include "client/main_client.h"
#include "server/main_server.h"

int main(int argc, char* argv[]) {
    if(argc < 2){
        printf("Potrebny je aspon 1 argument.");
        return 1;
    }
    if(argc == 2){
        main_server(argc, argv);
    } else if(argc == 3){
        main_client(argc, argv);
    } else {
        printf("Nespravne parametre");
        return 1;
    }
    return 0;
}