CC = g++
FLAGS = -Wall -Wextra -std=c++11


flyBasic: main.o calc.o
	$(CC) main.o calc.o -o superCalc


main.o: main.cpp
	$(CC) -c main.cpp $(FLAGS)


calc.o: calc.cpp
	$(CC) -c calc.cpp $(FLAGS)