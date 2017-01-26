#ifndef go_h
#define go_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "listeChainee.h"
#include "dessine.h"

void draw_win(void);
void mouse_clicked(int bouton, int x, int y);
void key_pressed(KeySym code, char c, int x_souris, int y_souris);
// Initialise une structure jeu
Jeu initJeu(int taille);
//Initialise un Pion
Pion * initPion(Couleur couleur);
Coord initCoord(int x, int y);
// Renvoie le nombre de degré de liberté d'un pion
int nbDegreLibertePion(Jeu jeu, Pion pion);
// Renvoie le nombre de degré de liberté d'une chaine
int nbDegreLiberteChaine(Jeu jeu,Liste liste);
//Joue un pion au coordonées coord de la couleur couleur
void playMoove(Jeu jeu, Coord coord, Couleur couleur);
//Verifie si un coup est authorisé
bool isAuthorizedMoove(Jeu jeu, Coord futurMoove);
void enleverPion(Jeu jeu, Pion * pion);

void fusionneChaineVoisine(Liste* chaine);
//Lance le jeux
void game(int argc, char *argv[]);

#endif /* go_h */
