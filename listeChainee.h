#ifndef listechainee_h
#define listechainee_h

#include <stdio.h>
#include <stdlib.h>
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

//Pour gérer la réfrence circulaire
typedef struct sList Liste;
typedef struct PionDuPlateau Pion;
typedef struct sNoeud Noeud;

//sert a stocker les informations pour les avoir dans l'ordre pour sauvegarder la partie
typedef struct DeroulementDeLaPartie
{
	int taillePlateau;
	char* nomJoueurBlanc;
	char* nomJoueurNoir;
	Liste* listePionNoir; //pion noir pausé dans l'ordre
	Liste* listePionBlanc; //pion blanc dans l'ordre
	int nbRound; //nombre de round
	bool isFinish; //sert a savoir si la partie est fini ou pas
	char* result; //sert a stocker dans le fichier le resultat final
}DeroulementPartie;

struct PionDuPlateau {
 Couleur couleur;
 Liste * chaineLie;
 Coord * coord;
};

typedef struct JeuDeGo {
 Coord * lastCoordBlanc;
 Coord * lastCoordNoir;
 Pion ** plateau;
 int taille;
 Couleur joueurCourant;
 bool isFinish;
}Jeu;

struct sNoeud
{
	Pion * pion;
	struct sNoeud* next;
	struct sNoeud* prev;
};


struct sList
{
	 int nb; //nombre de noeud
	 Noeud* first;
	 Noeud* last;
};


Noeud* nouveau_noeud(Pion * pion);

Liste* liste_vide(void);

void clear(Liste* l);

Noeud* push_front(Liste* l, Pion * pion);

void pop_front(Liste* l);

Pion * front_val(Liste* l);

void print(Liste* l);

int est_vide(Liste* l);

Noeud* trouve_premier(Liste* l, Pion pion);

int occurence(Liste* l, Pion pion);

Noeud* push_back(Liste* l, Pion * pion);

Pion * back_val(Liste* l);

void pop_back(Liste* l);

Noeud* insert_after(Liste* l, Noeud* c, Pion pion);

void retire(Liste* l, Noeud* n);

void removeAllChaine(Liste* chaine1);

void mergeChaine(Liste* chaine1, Liste* chaine2);

//void apply(Liste* l, void (*fonct)(int*));

#endif /* listechainee_h */
