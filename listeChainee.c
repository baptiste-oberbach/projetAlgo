#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "listeChainee.h"


//Cree un noeud
Noeud* nouveau_noeud(Pion * pion)
{
	Noeud* n = malloc(sizeof(Noeud));
	n->pion = pion;
	n->next = NULL;
	n->prev = NULL;
	return n;
}

//Genere une liste vide
Liste* liste_vide(void)
{
	Liste* l = malloc(sizeof(Liste));
	l->nb = 0;
	l->first = NULL;
	l->last = NULL;
	return l;
}

//Detruit toute la liste petit a petit
void clear(Liste* l)
{
	while (l->nb != 0)
		pop_front(l);
	free(l);
}

//Ajout d'un element en tete de la liste
Noeud* push_front(Liste* l, Pion pion)
{
	Noeud* n = nouveau_noeud(&pion);

	if (l->first == NULL)
	{
		l->first = n;
		l->last = n;
	}
	else
	{
		n->next = l->first;
		n->next->prev = n;
		l->first = n;
	}
	l->nb++;
	return n;
}

//Retire l'element en tete
void pop_front(Liste* l)
{
	Noeud* f = l->first;

	if (f == NULL)
		return;

	if (f == l->last) //(or nb==1)
	{
		l->first = NULL;
		l->last = NULL;
	}
	else
	{
		l->first = f->next;
		f->next->prev = NULL;
	}
	l->nb--;
	free(f);
}

//Retourne la valeur de la tete
Pion * front_val(Liste* l)
{
	assert(l->first != NULL);
	return l->first->pion;
}

//Ajout l'element en fin de liste
Noeud* push_back(Liste* l, Pion pion)
{
	Noeud* n = nouveau_noeud(&pion);

	if (l->last == NULL)
	{
		l->last = n;
		l->first = n;
		l->nb = 1;
		return n;
	}

	n->prev = l->last;
	l->last->next = n;
	l->last = n;
	l->nb++;

	return n;
}

//Retire le dernier noeud de la liste
void pop_back(Liste* l)
{
	Noeud* la = l->last;

	if (la == NULL)
		return;

	if (la == l->first) //(or nb==1)
	{
		l->first = NULL;
		l->last = NULL;
	}
	else
	{
		l->last = la->prev;
		la->prev->next = NULL;
	}
	l->nb--;
	free(la);
}

//Retourne la derniere valeur de la liste
Pion * back_val(Liste* l)
{
	assert(l->last != NULL);
	return l->last->pion;
}


//affiche la liste
void print(Liste* l)
{
	Noeud* n = l->first;
	while (n!=NULL)
	{
		printf("%d / ",n->pion->couleur);
		printf("%d / ",n->pion->coord.x);
		printf("%d / ",n->pion->coord.y);
		n = n->next;
	}
	puts("");
}

//retourne si la liste est vide ou non
int est_vide(Liste* l)
{
	return l->first == NULL;
}

//Retourne le premier noeud avec la valeur indiqué
/*Noeud* trouve_premier(Liste* l, Pion pion)
{
	Noeud* no = l->first;
	while (no!=NULL)
	{
		if (no->pion == pion)
			return no;
		no = no->next;
	}
	return NULL;
}*/

//Retourne le dernier noeud avec la valeur indiqué
/*Noeud* trouve_dernier(Liste* l, Pion pion)
{
	Noeud* no = l->last;
	while (no!=NULL)
	{
		if (no->pion == pion)
			return no;
		no = no->prev;
	}
	return NULL;
}*/

//compte le nombre d'occurence de la valeur dans la liste
/*int occurence(Liste* l, Pion pion)
{
	int nb=0;
	Noeud* no = l->first;
	while (no!=NULL)
	{
		if (no->pion == pion)
			nb++;
		no = no->next;
	}
	return nb;
}*/

//Insert un noeud avec la valeur donné après le noeud donné
Noeud* insert_after(Liste*l, Noeud* c, Pion pion)
{
	Noeud* n = nouveau_noeud(&pion);
	n->next = c->next;
	c->next = n;
	n->prev = c;

	if (c == l->last)
		l->last = n;
	else
		n->next->prev = n;

	l->nb--;

	return n;
}

//Retire le noeud spécifié de la liste
void retire(Liste* l, Noeud* n)
{
	if (l->first==NULL)
		return;
	if (n == l->first)
		pop_front(l);
	else if (n == l->last)
		pop_back(l);
	else
	{
		n->next->prev = n->prev;
		n->prev->next = n->next;
		free(n);
	}
}

//Permet de supprimer une chaine du plateau (visuel > vide + free de la chaine)
void removeAllChaine(Liste* chaine1)
{
	//Parcours la chaine jusqu'à l'avoir entièrement vidé
	while(chaine1->first != NULL)
	{
		Noeud* currentNode = chaine1->last;
		currentNode->pion->couleur = VIDE;
		retire(chaine1, currentNode);
	}
}

//Merge la chaine1 avec la chaine2
void mergeChaine(Liste* chaine1, Liste* chaine2)
{
	if(chaine1->first == NULL) //la premiere chaine est vide
		return;
	if(chaine2->first == NULL)
		return;

	chaine1->last->next = chaine2->first; //met a la suite pour lui enfiler l'autre chaine
	chaine2->first->prev = chaine1->last;
}

/*
void apply(Liste* l, void (*fonct)(int*))
{
	Noeud* n = l->first;
	while (n!=NULL)
	{
		fonct(&(n->val));
		n = n->next;
	}
}
*/
