CC = g++
FLAGS = -std=c++11 -ljsoncpp
COMPILE_FLAGS = $(FLAGS) -c 

all: main.out

main.out: main.o tools.o
	$(CC) main.o tools.o $(FLAGS) -o main.out

main.o: main.cpp tools.h
	$(CC) $(COMPILE_FLAGS) main.cpp

client.o: client.cpp tools.h
	$(CC) $(COMPILE_FLAGS) client.cpp

.PHONY: clean

clean:
				rm *.o
				rm *.out