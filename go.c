#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "go.h"
#include "dessine.h"


int taillePlateau = 19;
Jeu jeu;


/**
 * Mettre ici son code pour dessiner dans la fenetre
 * 1er affichage + redessine si resize
 */
void draw_win()
{
	// vide la fenetre
	clear_win();
	// Calcul des unités
	int uniteHauteur = height_win()/(taillePlateau+1);
	int uniteLargeur = width_win()/(taillePlateau+1);

	int i,j;
	//Choix de la couleur noir
	color(0.0,0.0,0.0);
	//traçage des lignes
	for(i=0; i<taillePlateau; i++)
	{
    	line(uniteLargeur,(i+1)*uniteHauteur,width_win()-uniteLargeur,(i+1)*uniteHauteur);
	}
	//traçage des colonnes
	for(j=0; j<taillePlateau; j++)
	{
		line((j+1)*uniteLargeur,uniteHauteur,(j+1)*uniteLargeur,height_win()-uniteHauteur);
	}

  	//traçage des hoshis
	switch(taillePlateau)
	{
		case 19:
			for(i=3; i<taillePlateau; i+=6)
			{
				for(j=3; j<taillePlateau; j+=6)
					filled_circle((i+1)*uniteLargeur,(j+1)*uniteHauteur,5);
			}
			break;
		case 13:
			for(i=3; i<taillePlateau; i+=6)
			{
				for(j=3; j<taillePlateau; j+=6)
					filled_circle((i+1)*uniteHauteur,(j+1)*uniteLargeur,5);
			}
			//hoshi central
			filled_circle(7*uniteHauteur,7*uniteLargeur,5);
			break;
		case 9:
			for(i=2; i<taillePlateau; i+=4)
			{
				for(j=2; j<taillePlateau; j+=4)
					filled_circle((i+1)*uniteHauteur,(j+1)*uniteLargeur,5);
			}
			//hoshi central
			filled_circle(5*uniteHauteur,5*uniteLargeur,5);
			break;
	}

	//Dessin des pions
	//Pour chaque croisement
	for(int i = 0;i < taillePlateau; i++)
	{
		for(int j = 0; j < taillePlateau; j++)
		{
			//Si il existe un pion sur ce croisement
			if(jeu.plateau[i*taillePlateau+j].couleur != VIDE)
			{
				//Choix de la couleur
				if(jeu.plateau[i*taillePlateau+j].couleur == BLANC)
				{
					color(1.0,1.0,1.0);
				}
				else
				{
					color(0.0,0.0,0.0);
				}
				filled_circle((j+1)*uniteHauteur,(i+1)*uniteLargeur,10);
			}
		}
	}
}


/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y)
{
	printf("Bouton %d presse au coord. %d,%d \n",bouton,x,y);
	//Si clique gauche
	if(bouton == 1)
	{
		double uniteHauteur = height_win()/(taillePlateau+1);
		double uniteLargeur = width_win()/(taillePlateau+1);

		double dX = x/uniteLargeur;
		double dY = y/uniteHauteur;
		// Calcul de la position dans le plateau
		int posX = floor(dX+0.5)-1;
		int posY = floor(dY+0.5)-1;
		// Cas des bords du tableau
		posX = posX<0 ? 0 :posX;
		posY = posY<0 ? 0 :posY;

		printf("largeur :%d hauteur :%d \n",posX,posY);
		Coord coord = initCoord(posX,posY);

		if(isAuthorizedMoove(jeu, coord))
		{
			playMoove(jeu,coord,jeu.joueurCourant);
			if(jeu.joueurCourant == BLANC)
			{
				jeu.lastCoordBlanc = coord;
				jeu.joueurCourant = NOIR;
			}
			else
			{
				jeu.lastCoordNoir = coord;
				jeu.joueurCourant = BLANC;
			}
		}
		
		draw_win();
		/*
		color( 0.0,0.0,0.0);
		filled_circle((posX+1)*uniteLargeur,(posY+1)*uniteHauteur,10);
		*/
	}
	//Si clique droit
	if(bouton == 3)
	{
		// Si les deux joueurs ont passé l'un après l'autre
		if((jeu.joueurCourant == BLANC && jeu.lastCoordNoir.x == -1) || (jeu.joueurCourant == NOIR && jeu.lastCoordBlanc.x == -1))
		{
			printf("Deux passage consécutif, fin du jeux \n");
		}
		else
		{
			//Le joueur Blanc passe
			if(jeu.joueurCourant == BLANC)
			{
				jeu.joueurCourant = NOIR;
				jeu.lastCoordBlanc = initCoord(-1,-1);
			}
			//Le joueur Noir passe
			else
			{
				jeu.joueurCourant = BLANC;
				jeu.lastCoordNoir = initCoord(-1,-1);
			}
		}
	}

}


/**
 * on a appuyé sur une touche
 * code: code touche x11 (XK_...)
 * c caractère correspondant si caractere
 * x_souris,y_souris position de la souris
 */
void key_pressed(KeySym code, char c, int x_souris, int y_souris)
{
	switch(code)
	{
		case XK_Down:
			printf("bas\n");
			break;
		case XK_Up:
			printf("haut\n");
			break;
		case XK_Left:
			printf("gauche\n");
			break;
		case XK_Right:
			printf("droite\n");
			break;
	//~ case XK_Return:
	//~ case XK_Shift_L:
	//~ case XK_Control_R:
	//~ ....
		default:
			break;
	}

	if (c>' ' && c<'z')
		printf("char: %c \n",c);

	printf(" avec pos souris: %d,%d \n",x_souris,y_souris);

}
// Initialise une structure jeu
Jeu initJeu(int taille)
{
	Pion * plateau= malloc(sizeof(Pion*)* taille * taille);
	//Initialise le plateau pour chaque croisement
	for(int i = 0;i < taille; i++)
	{
		for(int j = 0; j < taille; j++)
		{
			plateau[i*taille+j]=initPion(VIDE);
		}
	}
	Jeu jeu;
	jeu.plateau = plateau;
	jeu.lastCoordBlanc = initCoord(-2,-2);
	jeu.lastCoordNoir = initCoord(-2,-2);
	jeu.taille = taille;
	jeu.joueurCourant = NOIR;
	return jeu;
}

//Initialise un Pion
Pion initPion(Couleur couleur)
{
	Pion pion;
	pion.couleur = couleur;
	return pion;
}

Coord initCoord(int x, int y)
{
	Coord coord;
	coord.x = x;
	coord.y = y;
	return coord;
}



bool checkDegreLibertePion(Jeu jeu, Pion pion)
{
	//Check la case au dessus
	if(pion.coord.y != 0 && jeu.plateau[pion.coord.y-1*jeu.taille + pion.coord.x].couleur != VIDE)
	{
		return false;
	}
	//Check la case en dessus
	if(pion.coord.y != jeu.taille-1 &&  jeu.plateau[pion.coord.y+1*jeu.taille + pion.coord.x].couleur != VIDE)
	{
		return false;
	}
	//Check la case à droite
	if(pion.coord.x != jeu.taille-1 &&  jeu.plateau[pion.coord.x-1 + pion.coord.y*jeu.taille].couleur != VIDE)
	{
		return false;
	}
	//Check la case à gauche
	if(pion.coord.x != 0 &&  jeu.plateau[pion.coord.x+1 + pion.coord.y*jeu.taille].couleur != VIDE)
	{
		return false;
	}
	return true;
}

//check si cest un mouvement autorisé 
bool isAuthorizedMoove(Jeu jeu, Coord futurMoove)
{
	//check si la case n'est deja occupé par un autre pion
	if (jeu.plateau[futurMoove.y*jeu.taille + futurMoove.x].couleur == VIDE)
	{
		//check le cas de répétition
		if(jeu.joueurCourant == NOIR)
		{
			if(futurMoove.y == jeu.lastCoordBlanc.y && futurMoove.x == jeu.lastCoordBlanc.x)
				return false;	

		}
		else //c'est le joueur blanc
		{
			if(futurMoove.y == jeu.lastCoordNoir.y && futurMoove.x == jeu.lastCoordNoir.x)
				return false;
		}
		return true;
	}
	else
	{
		//deja un pion sur cette case, pas le droit de faire ca
		return false;
	}
}

void playMoove(Jeu jeu, Coord coord, Couleur couleur)
{
	//TODO Check if moove is allowed

	Pion pion = initPion(couleur);
	pion.coord = coord;
	jeu.plateau[coord.x + coord.y * jeu.taille] = pion;
}

//Lance le jeu
void game(int argc, char *argv[])
{
	int largeur = 700;
	int hauteur = 700;
	for(int i =1;i<argc;i+=2)
	{
		// Si l'argument est -largeur
		if(strcmp(argv[i],"-largeur") == 0)
		{
			largeur = atoi(argv[i+1]);
		}
		// Si l'argument est -largeur
		if(strcmp(argv[i],"-hauteur") == 0)
		{
			hauteur = atoi(argv[i+1]);
		}
	}
	init_win(largeur,hauteur, "Jeu de GO",246,254,185);
	jeu = initJeu(taillePlateau);
  	event_loop();
}
