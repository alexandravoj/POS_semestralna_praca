cmake_minimum_required(VERSION 3.18)
project(POS_semestralna_praca)

set(CMAKE_CXX_STANDARD 17)

add_executable(POS_semestralna_praca main.cpp client/main_client.cpp spravca/file_handler.h spravca/file_handler.cpp
        logika/bunka.h logika/game_of_life.h logika/game_of_life.cpp
        simulacia/thread_data.h
        simulacia/thread_data.cpp
        client/client.cpp
        client/client.h
        server/main_server.cpp
        server/server.cpp
        server/server.h)

target_link_libraries(POS_semestralna_praca pthread)

