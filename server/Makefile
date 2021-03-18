CC = g++
FLAGS = -std=c++11 -ljsoncpp
COMPILE_FLAGS = $(FLAGS) -c 

all: server

server: main.o tools.o
	$(CC) main.o tools.o $(FLAGS) -o server

main.o: main.cpp tools.h
	$(CC) $(COMPILE_FLAGS) main.cpp

tools.o: tools.cpp tools.h
	$(CC) $(COMPILE_FLAGS) tools.cpp

.PHONY: clean

clean:
	rm *.o server