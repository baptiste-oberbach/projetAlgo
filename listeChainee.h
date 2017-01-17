#ifndef listechainee_h
#define listechainee_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct sNoeud
{
	Pion pion;
	struct sNoeud* next;
	struct sNoeud* prev;
} Noeud;
	

typedef struct
{
	 int nb; //nombre de noeud
	 Noeud* first;
	 Noeud* last;
} Liste;


Noeud* nouveau_noeud(Pion pion);

Liste* liste_vide(void);

void clear(Liste* l);

Noeud* push_front(Liste* l, Pion pion);

void pop_front(Liste* l);

Pion front_val(Liste* l);

void print(Liste* l);

int est_vide(Liste* l);

Noeud* trouve_premier(Liste* l, Pion pion);

int occurence(Liste* l, Pion pion);

Noeud* push_back(Liste* l, Pion pion);

Pion back_val(Liste* l);

void pop_back(Liste* l);

Noeud* insert_after(Liste* l, Noeud* c, Pion pion);

void retire(Liste* l, Noeud* n);

void mergeChaine(Liste* chaine1, Liste* chaine2);

//void apply(Liste* l, void (*fonct)(int*));

#endif /* listechainee_h */