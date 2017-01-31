CC = gcc

LFLAGS = -std=c99 -g -W -Wall -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wbad-function-cast -Wcast-qual -o

CFLAGS = -std=c99 -g -W -Wall -Wmissing-declarations -Wmissing-prototypes -Wredundant-decls -Wbad-function-cast -Wcast-qual

LIBS = -lm -lX11

SRC= testGo.c listeChainee.c dessine.c go.c

all : testGo

% : %.c $(SRC)
	$(CC) $(LFLAGS) $@ $^ $(LIBS)

clean:
	rm -f *.o

clear: clean
	rm -f $(EXEC)
