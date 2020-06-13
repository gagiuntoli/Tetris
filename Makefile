all: main

%: src/%.cpp
	g++ $^ -o $@ -lcurses
