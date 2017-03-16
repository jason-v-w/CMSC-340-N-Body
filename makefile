firstmake: bodies.c main.c
	gcc -g -o app bodies.c main.c gfx.c filereader.c -lm -lX11 -pthread -I.
