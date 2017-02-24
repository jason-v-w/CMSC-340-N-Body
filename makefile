firstmake: bodies.c main.c
	gcc -o app bodies.c main.c -lm -pthread -I.
	
