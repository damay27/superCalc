CC = g++
FLAGS = -Wall -Wextra -std=c++11


flyBasic: main.o program.o
	$(CC) main.o program.o -o superCalc


main.o: main.cpp
	$(CC) -c main.cpp $(FLAGS)


program.o: program.cpp
	$(CC) -c program.cpp $(FLAGS)