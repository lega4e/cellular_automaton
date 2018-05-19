CC=g++
CFLAGS=-c -Wall -O3
LDFLAGS=
LIBS=-lclever-sfmlwidgets -lsfml-graphics -lsfml-window -lsfml-system
SOURCES=main.cpp CellularAutomaton.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=cellular_automaton

all: $(SOURCES) $(EXECUTABLE)
	$(EXECUTABLE)

reall: clearall all
	

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


clear:
	rm *.o

clearall: clear
	rm $(EXECUTABLE)
