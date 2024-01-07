CXX = g++
CXXFLAGS = -std=c++11 -Wall

all: main

main: main.o main_client.o main_server.o FileHandler.o client.o server.o threadData.o gameOfLife.o
	$(CXX) $(CXXFLAGS) $^ -o $@ -pthread

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main_client.o: client/main_client.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

main_server.o: server/main_server.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

FileHandler.o: spravca/file_handler.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
client.o: client/client.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

server.o: server/server.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

threadData.o: simulacia/thread_data.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

gameOfLife.o: logika/game_of_life.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

server: main
	./main 10011

client: main
	./main frios2.fri.uniza.sk 10011

clean:
	rm -f *.o main

