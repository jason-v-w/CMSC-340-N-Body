firstmake: bodies.c nbody.c main.c
	gcc -o app bodies.c nbody.c main.c -lm -pthread -I.
	
