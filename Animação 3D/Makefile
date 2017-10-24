CC = g++
coisasGlut = -lGL -lGLU -lglut
#coisasTiny = tinystr.cpp tinyxml.cpp tinyxmlerror.cpp tinyxmlparser.cpp

all: 
	$(CC) -w -O4 -c main.cpp
	$(CC) -g -O4 -s main.o $(coisasGlut) -lm -o trabalhocg
	./trabalhocg

run:
	./trabalhocg

clean:
	rm *.o
	rm trabalhocg
	reset



