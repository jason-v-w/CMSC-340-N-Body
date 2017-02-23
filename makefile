firstmake: bodies.c testing.c
	gcc -o app bodies.c testing.c -lm -I.
