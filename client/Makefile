CC = g++
FLAGS = -std=c++11 -ljsoncpp
COMPILE_FLAGS = $(FLAGS) -c 

all: client

client: main.o
	$(CC) main.o $(FLAGS) -o client

main.o: main.cpp client.h
	$(CC) $(COMPILE_FLAGS) main.cpp

.PHONY: clean

clean:
	rm *.o
	rm client
