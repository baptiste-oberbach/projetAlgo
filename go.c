#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "go.h"
#include "dessine.h"


int taillePlateau = 19;
int * plateau;


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
}


/**
 * on a cliqué a la souris:
 * bouton: 1,2,3,4,5,... : gauche, milieu, droit, molette, ...
 * x,y position
 */
void mouse_clicked(int bouton, int x, int y)
{
	printf("Bouton %d presse au coord. %d,%d \n",bouton,x,y);

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
	color( 0.0,0.0,0.0);
	filled_circle((posX+1)*uniteLargeur,(posY+1)*uniteHauteur,10);
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
  init_win(largeur,hauteur, "Essai",246,254,185);
  event_loop();
}
