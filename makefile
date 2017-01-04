arbremake: testGo.c go.c
	gcc -o goExec testGo.c go.c -I.
