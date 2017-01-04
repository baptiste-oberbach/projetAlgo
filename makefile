goExec: testGo.c go.c dessine.c
	gcc -o goExec testGo.c go.c dessine.c -I. -lX11
