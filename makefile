goExec: testGo.c go.c dessine.c
	gcc -o goExec testGo.c go.c dessine.c -I. -lX11 -lm -std=c99 -Wall -Wextra
