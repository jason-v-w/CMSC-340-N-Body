firstmake: bodies.c nbody.c main.c window.cpp
	gcc -o app bodies.c nbody.c main.c -lm -pthread -I.
	g++ -c window.cpp
	g++ window.o -o window -lsfml-graphics -lsfml-window -lsfml-system

	gcc -c -Wall -Werror -fPIC bodies.c nbody.c
	gcc -shared -o libCfile.so bodies.o nbody.o
