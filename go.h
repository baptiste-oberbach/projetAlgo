#ifndef go_h
#define go_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dessine.h"
#include <math.h>

typedef struct Coordonnees {
 int x;
 int y;
}
Coord;

typedef struct PionDuPlateau {
 int couleur;
 struct PionDuPlateau * chaineLie;
}
Pion;

typedef struct JeuDeGo {
 Coord lastCoordBlanc;
 Coord lastCoordNoir;
 Pion ** plateau;
}
Jeu;

void draw_win();
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
void game(int argc, char *argv[]);

#endif /* go_h */
