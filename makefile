firstmake: bodies.c testing.c
	gcc -o app bodies.c main.c -lm -I.
	
