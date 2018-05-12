all: test.out

cellular_automaton.out: main.cpp
	g++ -o cellular_automaton.out main.cpp -lsfml-graphics -lsfml-window -lsfml-system

test.out: test.cpp SFMLFieldAdapter.hpp
	g++ -o test.out test.cpp -lsfml-graphics -lsfml-window -lsfml-system

clear:
	rm *.o
