#ifndef go_h
#define go_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dessine.h"
#include <math.h>
#include <stdbool.h>

typedef enum Couleur Couleur;
enum Couleur
{
    VIDE, BLANC, NOIR,
};

typedef struct Coordonnees {
 int x;
 int y;
}Coord;

typedef struct PionDuPlateau {
 Couleur couleur;
 struct PionDuPlateau* chaineLie;
 Coord coord;
}Pion;

typedef struct JeuDeGo {
 Coord lastCoordBlanc;
 Coord lastCoordNoir;
 Pion * plateau;
 int taille;
 Couleur joueurCourant;
}Jeu;

void draw_win(void);
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
// Initialise une structure jeu
Jeu initJeu(int taille);
//Initialise un Pion
Pion initPion(Couleur couleur);
Coord initCoord(int x, int y);
bool checkDegreLibertePion(Jeu jeu, Pion pion);
void playMoove(Jeu jeu, Coord coord, Couleur couleur);
bool isAuthorizedMoove(Jeu jeu, Coord futurMoove);
void game(int argc, char *argv[]);

#endif /* go_h */
