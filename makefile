firstmake: bodies.c testing.c
	gcc -o app bodies.c testing.c -lm -I.
	g++ -c window.cpp
	g++ window.o -o window -lsfml-graphics -lsfml-window -lsfml-system

	gcc -c -Wall -Werror -fPIC bodies.c
	gcc -shared -o libCfile.so bodies.o
