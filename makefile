firstmake: bodies.c main.c
	gcc -o app bodies.c main.c gfx.c -lm -lX11 -pthread -I.
	
