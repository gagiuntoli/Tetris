all: main

main: src/main.cpp
	g++ src/main.cpp -o main -lcurses
