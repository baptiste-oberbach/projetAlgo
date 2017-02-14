#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include "go.h"
#include "listeChainee.h"
#include "dessine.h"
#include "sgfSaveHelper.h"




int taillePlateau = 19;
bool ia = false;
Jeu * jeu;
DeroulementPartie* deroulementPartie;


/**
 * Mettre ici son code pour dessiner dans la fenetre
 * 1er affichage + redessine si resize
 */
void draw_win()
{
	// vide la fenetre
	clear_win();
	// Calcul des unités
	int uniteHauteur = height_win()/(jeu->taille+1);
	int uniteLargeur = width_win()/(jeu->taille+1);

	int i,j;
	//Choix de la couleur noir
	color(0.0,0.0,0.0);
	//traçage des lignes
	for(i=0; i<jeu->taille; i++)
	{
    	line(uniteLargeur,(i+1)*uniteHauteur,width_win()-uniteLargeur,(i+1)*uniteHauteur);
	}
	//traçage des colonnes
	for(j=0; j<jeu->taille; j++)
	{
		line((j+1)*uniteLargeur,uniteHauteur,(j+1)*uniteLargeur,height_win()-uniteHauteur);
	}

  	//traçage des hoshis
	switch(jeu->taille)
	{
		case 19:
			for(i=3; i<jeu->taille; i+=6)
			{
				for(j=3; j<jeu->taille; j+=6)
					filled_circle((i+1)*uniteLargeur,(j+1)*uniteHauteur,5);
			}
			break;
		case 13:
			for(i=3; i<jeu->taille; i+=6)
			{
				for(j=3; j<jeu->taille; j+=6)
					filled_circle((i+1)*uniteHauteur,(j+1)*uniteLargeur,5);
			}
			//hoshi central
			filled_circle(7*uniteHauteur,7*uniteLargeur,5);
			break;
		case 9:
			for(i=2; i<jeu->taille; i+=4)
			{
				for(j=2; j<jeu->taille; j+=4)
					filled_circle((i+1)*uniteHauteur,(j+1)*uniteLargeur,5);
			}
			//hoshi central
			filled_circle(5*uniteHauteur,5*uniteLargeur,5);
			break;
	}

	//Dessin des pions
	//Pour chaque croisement
	for(int i = 0;i < jeu->taille; i++)
	{
		for(int j = 0; j < jeu->taille; j++)
		{
			//Si il existe un pion sur ce croisement
			if(jeu->plateau[i*jeu->taille+j]->couleur != VIDE)
			{
				//Choix de la couleur
				if(jeu->plateau[i*jeu->taille+j]->couleur == BLANC)
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
	if(jeu->isFinish)
	{
		//phase ou on enleve les pions conflictuels

		//Si clique gauche
		if(bouton == 1)
		{
		}

	}else{
		//Si clique gauche
		if(bouton == 1)
		{
			double uniteHauteur = height_win()/(jeu->taille+1);
			double uniteLargeur = width_win()/(jeu->taille+1);

			double dX = x/uniteLargeur;
			double dY = y/uniteHauteur;
			// Calcul de la position dans le plateau
			int posX = floor(dX+0.5)-1;
			int posY = floor(dY+0.5)-1;
			// Cas des bords du tableau
			posX = posX<0 ? 0 :posX;
			posY = posY<0 ? 0 :posY;
			printf(" x : %d y %d \n",posX,posY);
			posX = posX>jeu->taille-1 ? jeu->taille-1 :posX;
			posY = posY>jeu->taille-1 ? jeu->taille-1 :posY;

			Coord * coord = initCoord(posX,posY);

			if(isAuthorizedMoove(jeu, *coord))
			{
				playMoove(jeu,coord,jeu->joueurCourant);
				//Fais jouer l'ia
				if(ia)
				{
					playMooveIA(jeu);
				}
			}
			else
			{
				free(coord);
			}

			draw_win();
		}
		//Si clique droit
		if(bouton == 3)
		{
			passe(jeu);
			playMooveIA(jeu);
			draw_win();
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

	switch(c)
	{
		case 'S':
		case 's':
			saveGame();
			break;

	}

}
// Initialise une structure jeu
Jeu * initJeu(int taille)
{
	printf("Start to Initialise game \n");
	Pion ** plateau= malloc(sizeof(Pion*)* taille * taille);
	//Initialise le plateau pour chaque croisement
	for(int i = 0;i < taille; i++)
	{
		for(int j = 0; j < taille; j++)
		{
			Pion * pion = initPion(VIDE);
			pion->coord = initCoord(j,i);
			plateau[i*taille+j] = pion;
		}
	}
	Jeu * jeu = malloc(sizeof(Jeu));
	jeu->plateau = plateau;
	jeu->lastCoordBlanc = initCoord(-2,-2);
	jeu->lastCoordNoir = initCoord(-2,-2);
	jeu->taille = taille;
	jeu->joueurCourant = NOIR;
	jeu->isFinish = false;

	return jeu;
}

DeroulementPartie* initDeroulementPartie(Jeu * jeu)
{
	DeroulementPartie* deroulementPartie = malloc(sizeof(DeroulementPartie));
	printf("Apres malloc ?\n");
	printf("jeu->taille %d\n", jeu->taille);
	deroulementPartie->taillePlateau = jeu->taille;
	printf("deroulementPartie->taillePlateau %d \n", deroulementPartie->taillePlateau);
	deroulementPartie->nomJoueurBlanc = "Joueur blanc";
	printf("deroulementPartie->nomJoueurBlanc %s\n", deroulementPartie->nomJoueurBlanc);
	deroulementPartie->nomJoueurNoir = "Joueur noir";
	printf("deroulementPartie->nomJoueurNoir %s\n", deroulementPartie->nomJoueurNoir);
	deroulementPartie->listePionNoir = liste_vide(); //pion noir pausé dans l'ordre
	deroulementPartie->listePionBlanc = liste_vide(); //pion blanc dans l'ordre
	printf("apres init des listes\n");
	deroulementPartie->nbRound = 0; //nombre de round
	printf("deroulementPartie->nbRound %d\n", deroulementPartie->nbRound);
	deroulementPartie->result = "";
	printf("deroulementPartie->result %s\n", deroulementPartie->result);
	deroulementPartie->isFinish = false;
	printf("deroulementPartie->isFinish %d\n", deroulementPartie->isFinish);

	return deroulementPartie;
}

//Initialise un Pion
Pion * initPion(Couleur couleur)
{
	Pion * pion = malloc(sizeof(Pion));
	pion->couleur = couleur;
	return pion;
}

Coord * initCoord(int x, int y)
{
	Coord * coord = malloc(sizeof(coord));
	coord->x = x;
	coord->y = y;
	return coord;
}

int nbDegreLibertePion(Jeu * jeu, Pion * pion)
{
	int res = 0;
	printf("check degre liberte du pion x:%d y:%d\n",pion->coord->x, pion->coord->y );
//	printf("pion coord y %d taille %d pion coord y %d \n",pion->coord->y, );
	//Check la case au dessus
	if(pion->coord->y != 0 && jeu->plateau[(pion->coord->y-1)*jeu->taille + pion->coord->x]->couleur == VIDE)
	{
		res++;
	}
	//Check la case en dessus
	if(pion->coord->y != jeu->taille-1 &&  jeu->plateau[(pion->coord->y+1)*jeu->taille + pion->coord->x]->couleur == VIDE)
	{
		res++;
	}
	//Check la case à droite
	if(pion->coord->x != jeu->taille-1 &&  jeu->plateau[pion->coord->x+1 + pion->coord->y*jeu->taille]->couleur == VIDE)
	{
		res++;
	}
	//Check la case à gauche
	if(pion->coord->x != 0 &&  jeu->plateau[pion->coord->x-1 + pion->coord->y*jeu->taille]->couleur == VIDE)
	{
		res++;
	}
	return res;
}

int nbDegreLiberteChaine(Jeu * jeu,Liste liste)
{
	int res = 0;
	Noeud * noeud = liste.first;
	for(int i =0; i<liste.nb; i++)
	{
		res += nbDegreLibertePion(jeu,noeud->pion);
		noeud = noeud->next;
	}
	return res;
}

bool isAuthorizedSense(Jeu * jeu, Pion * adjacentPion)
{
	//Si la case est vide on peux jouer
	if(adjacentPion->couleur == VIDE)
	{
		return true;
	}
	//Si la case adjacente est de la même couleur on vérifie que la chaine créer garde une liberté
	else if(adjacentPion->couleur == jeu->joueurCourant)
	{
		//Check si chaine à coté à un degré de liberté > 1
		int nbDegre = nbDegreLiberteChaine(jeu,*(adjacentPion->chaineLie));
		printf("la chaine à %d degré de liberté\n", nbDegre);
		if(nbDegre > 0)
		{
			return true;
		}
	}
	//SI le pion adjacent est de la couleur opposé on regarde si placé le pion capture la chaine càd si la chaine n'a qu'une liberté
	else
	{
		int nbDegre = nbDegreLiberteChaine(jeu,*(adjacentPion->chaineLie));
		printf("la chaine à %d degré de liberté\n", nbDegre);
		if(nbDegre < 1)
		{
			return true;
		}
	}
	return false;
}

//check si cest un mouvement autorisé
bool isAuthorizedMoove(Jeu * jeu, Coord futurMoove)
{
	//Verif des repétition et de la case déjà occupé
	//check si la case n'est deja occupé par un autre pion
	if (jeu->plateau[futurMoove.y*jeu->taille + futurMoove.x]->couleur == VIDE)
	{
		//check le cas de répétition
		if(jeu->joueurCourant == NOIR)
		{
			if(futurMoove.y == jeu->lastCoordNoir->y && futurMoove.x == jeu->lastCoordNoir->x)
				return false;

		}
		else //c'est le joueur blanc
		{
			if(futurMoove.y == jeu->lastCoordBlanc->y && futurMoove.x == jeu->lastCoordBlanc->x)
				return false;
		}
	}
	else
	{
		//deja un pion sur cette case, pas le droit de faire ca
		return false;
	}
	printf("future moove x:%d y:%d \n",futurMoove.x,futurMoove.y);
	jeu->plateau[futurMoove.y*jeu->taille +futurMoove.x]->couleur = jeu->joueurCourant;
	Pion  * adjacentPion;
	//cas d'une cases adjacente vide et n'étant pas le bord => on peux placer le pion

	//check la case au dessus
	if(futurMoove.y != 0)
	{
		adjacentPion = jeu->plateau[(futurMoove.y-1)*jeu->taille + futurMoove.x];
		if(isAuthorizedSense(jeu, adjacentPion))
		{
			jeu->plateau[futurMoove.y*jeu->taille +futurMoove.x]->couleur = VIDE;
			return true;
		}
	}
	//check la case en dessous
	if(futurMoove.y != jeu->taille)
	{
		adjacentPion = jeu->plateau[(futurMoove.y+1)*jeu->taille + futurMoove.x];
		if(isAuthorizedSense(jeu, adjacentPion))
		{
			jeu->plateau[futurMoove.y*jeu->taille +futurMoove.x]->couleur = VIDE;
			return true;
		}
	}
	// check la case à gauche
	if(futurMoove.x != 0)
	{
		adjacentPion = jeu->plateau[(futurMoove.y)*jeu->taille + futurMoove.x-1];
		if(isAuthorizedSense(jeu, adjacentPion))
		{
			jeu->plateau[futurMoove.y*jeu->taille +futurMoove.x]->couleur = VIDE;
			return true;
		}
	}

	//CHeck la case à droite
	if(futurMoove.x != jeu->taille)
	{
		adjacentPion = jeu->plateau[(futurMoove.y)*jeu->taille + futurMoove.x+1];
		if(isAuthorizedSense(jeu, adjacentPion))
		{
			printf("couleur joueur %d, couleur adjacent %d\n", jeu->joueurCourant, adjacentPion->couleur);
			jeu->plateau[futurMoove.y*jeu->taille +futurMoove.x]->couleur = VIDE;
			return true;
		}
	}
	jeu->plateau[futurMoove.y*jeu->taille +futurMoove.x]->couleur = VIDE;
	printf("Pas auth \n");
	return false;
}

void passe(Jeu * jeu)
{
	// Si les deux joueurs ont passé l'un après l'autre
	if((jeu->joueurCourant == BLANC && jeu->lastCoordNoir->x == -1) || (jeu->joueurCourant == NOIR && jeu->lastCoordBlanc->x == -1))
	{
		printf("Deux passage consécutif, fin du jeux \n");
		//Pour nous, le jeu est terminé, passe cette variable a true pour que les prochains cliques soient gérer différements (pour qu'on puisse enlever les pions conflictuelles du plateau)
		jeu->isFinish = true;
		finirPartie(jeu);
	}
	else
	{
		//Le joueur Blanc passe
		if(jeu->joueurCourant == BLANC)
		{
			jeu->joueurCourant = NOIR;
			jeu->lastCoordBlanc = initCoord(-1,-1);
		}
		//Le joueur Noir passe
		else
		{
			jeu->joueurCourant = BLANC;
			jeu->lastCoordNoir = initCoord(-1,-1);
		}
	}
}

void playMoove(Jeu * jeu, Coord * coord, Couleur couleur)
{
	Pion * pion = initPion(couleur);
	pion->coord = coord;
	//crée sa propre chaine
	Liste* selfChaine = liste_vide();
	push_front(selfChaine, pion); //met le pion dedans
	pion->chaineLie = selfChaine;
	fusionneChaineVoisine(jeu, pion);
	print(selfChaine);
	jeu->plateau[coord->x + coord->y * jeu->taille] = pion;

	//copie pion courant -> besoin pour la sauvegarde car les pions de notre plateau peuvent être supprimé de la mémoire si ils sont pris
	//du coup on fait une copie qui sera seulement dans la liste de deroulement de la partie
	Pion* copiePionCourant = initPion(couleur);
	copiePionCourant->chaineLie = liste_vide();
	copiePionCourant->coord = initCoord(coord->x, coord->y);
	printf("Copie pion courant %d couleur\n", copiePionCourant->couleur);
	printf("x %d y %d\n", copiePionCourant->coord->x, copiePionCourant->coord->y);

	//une fois le pion ajouté au jeu, on l'ajoute dans le deroulement de la partie
	if(couleur == NOIR)
	{
		push_back(deroulementPartie->listePionNoir, copiePionCourant);
	}
	else if(couleur == BLANC)
	{
		push_back(deroulementPartie->listePionBlanc, copiePionCourant);
	}
	deroulementPartie->nbRound += 1;


	Pion * adjacentPion;
	//Regarde si une chaine adjacente ennemie est capturé
	//check la case au dessus
	if(coord->y != 0)
	{
		adjacentPion = jeu->plateau[(coord->y-1)*jeu->taille + coord->x];
		//Test seulement si le pion du dessus est de la couleur opposé
		if(adjacentPion->couleur != VIDE && adjacentPion->couleur != jeu->joueurCourant)
		{
			printf("Check de la chaine : ");
			print(adjacentPion->chaineLie);
			int nbDegre = nbDegreLiberteChaine(jeu,*(adjacentPion->chaineLie));
			printf("il y a %d degré de liberté \n", nbDegre);
			if(nbDegre == 0)
			{
				enleverChaine(jeu,adjacentPion->chaineLie);
			}
		}
	}
	//check la case en dessous
	if(coord->y < jeu->taille-1)
	{
		adjacentPion = jeu->plateau[(coord->y+1)*jeu->taille + coord->x];
		if(adjacentPion->couleur != VIDE && adjacentPion->couleur != jeu->joueurCourant)
		{
			printf("Check de la chaine : ");
			print(adjacentPion->chaineLie);
			int nbDegre = nbDegreLiberteChaine(jeu,*(adjacentPion->chaineLie));
			printf("il y a %d degré de liberté \n", nbDegre);
			if(nbDegre == 0)
			{
				//Capture
				enleverChaine(jeu,adjacentPion->chaineLie);
			}
		}
	}
	// check la case à gauche
	if(coord->x != 0)
	{
		adjacentPion = jeu->plateau[(coord->y)*jeu->taille + coord->x - 1 ];
		if(adjacentPion->couleur != VIDE && adjacentPion->couleur != jeu->joueurCourant)
		{
			printf("Check de la chaine : ");
			print(adjacentPion->chaineLie);
			int nbDegre = nbDegreLiberteChaine(jeu,*(adjacentPion->chaineLie));
			printf("il y a %d degré de liberté \n", nbDegre);
			if(nbDegre == 0)
			{
				//Capture

				enleverChaine(jeu,adjacentPion->chaineLie);
			}
		}
	}

	//CHeck la case à droite
	if(coord->x != jeu->taille-1)
	{
		adjacentPion = jeu->plateau[(coord->y)*jeu->taille + coord->x + 1];
		if(adjacentPion->couleur != VIDE && adjacentPion->couleur != jeu->joueurCourant)
		{
			printf("Check de la chaine : ");
			print(adjacentPion->chaineLie);
			int nbDegre = nbDegreLiberteChaine(jeu,*(adjacentPion->chaineLie));
			printf("il y a %d degré de liberté \n", nbDegre);
			if(nbDegre == 0)
			{
				//Capture
				enleverChaine(jeu,adjacentPion->chaineLie);
			}
		}
	}
	if(jeu->joueurCourant == BLANC)
	{
		jeu->lastCoordBlanc = coord;
		jeu->joueurCourant = NOIR;
	}
	else
	{
		jeu->lastCoordNoir = coord;
		jeu->joueurCourant = BLANC;
	}
}

void enleverPion(Jeu * jeu, Pion * pion)
{
	Pion * newPion = initPion(VIDE);
	newPion->coord = pion->coord;
	free(jeu->plateau[pion->coord->x + pion->coord->y * jeu->taille]);
	jeu->plateau[pion->coord->x + pion->coord->y * jeu->taille] = newPion;
}


void enleverChaine(Jeu * jeu, Liste * liste)
{
	Noeud * noeud = liste->first;
	if(liste->nb==1)
	{
		enleverPion(jeu,noeud->pion);
	}
	else
	{
		for(int i =0; i< liste->nb;i++)
		{
			enleverPion(jeu,noeud->pion);
			noeud = noeud->next;
		}
		/*
		do
		{
			enleverPion(jeu,noeud->pion);
			noeud = noeud->next;
		}
		while(noeud != liste->last);*/
	}
	//Vide la chaine
	clear(liste);
	//removeAllChaine(liste);
}
/**
	Cette fonctionne va check tous les chaines de la meme couleur
	a coté d'un élement de la chaine, pour faire une seule chaine si plusieurs chaines
	sont autour
*/
void fusionneChaineVoisine(Jeu * jeu, Pion * pion)
{
	int x = pion->coord->x;
	int y = pion->coord->y;
	//Fusionne la chaine en haut
	Pion * adjacentPion;
	if(y > 0 && jeu->plateau[(y-1)*jeu->taille+x]->couleur == pion->couleur)
	{
		adjacentPion = jeu->plateau[(y-1)*jeu->taille+x];
		mergeChaine(pion->chaineLie, adjacentPion->chaineLie);
	}
	// Fusionne la chaine d'en bas
	if(y < jeu->taille-1 && jeu->plateau[(y+1)*jeu->taille+x]->couleur == pion->couleur)
	{
		adjacentPion = jeu->plateau[(y+1)*jeu->taille+x];
		mergeChaine(pion->chaineLie, adjacentPion->chaineLie);
	}
	// Fusionne la chaine à gauche
	if(x > 0 && jeu->plateau[(y*jeu->taille)+x-1]->couleur == pion->couleur)
	{
		adjacentPion = jeu->plateau[(y*jeu->taille)+x-1];
		mergeChaine(pion->chaineLie, adjacentPion->chaineLie);
	}
	// Fusionne la chaine à droite
	if(x < jeu->taille-1 && jeu->plateau[(y*jeu->taille)+x+1]->couleur == pion->couleur)
	{
		adjacentPion = jeu->plateau[(y*jeu->taille)+x+1];
		mergeChaine(pion->chaineLie, adjacentPion->chaineLie);
	}
}

void saveGame(void)
{
	writePartyData("gameSave.sgf", deroulementPartie);
}

void finirPartie(Jeu * jeu)
{
	afficheScore(jeu);
}

void afficheScore(Jeu * jeu)
{
	int scoreBlanc = 7;
	int scoreNoir = 0;
	//Calcul du score
	for(int i = 0; i< jeu->taille; i++)
	{
		for(int j = 0; j< jeu->taille; j++)
		{
			if(jeu->plateau[i * jeu->taille +j ]->couleur == BLANC)
				scoreBlanc ++;
			else if(jeu->plateau[i * jeu->taille +j ]->couleur == NOIR)
				scoreNoir ++;
		}
	}
	printf("Le score des noirs est de %d \n",scoreNoir);
	printf("Le score des blancs est de %d,5 \n",scoreBlanc);
	//Cas d'une victoire des noires
	if(scoreNoir > scoreBlanc)
		printf("Les noirs ont gagnés \n");
	else
	{
		printf("Les blancs ont gagnés \n");
	}
}

void playMooveIA(Jeu * jeu)
{
	srand(time(NULL));
	Coord * rdmCoord = randomCoord();
	int i = 0;
	//Recherche un coup aléatoire jouable
	while(!isAuthorizedMoove(jeu,*rdmCoord))
	{
	 rdmCoord = randomCoord();
	 i++;
	 //Si on ne trouve pas de coup au bout de 10 tentatives
	 if(i>10)
	 {
		 printf("L'IA passe son tour");
		 passe(jeu);
		 return;
	 }
	}
	printf("L'IA joue en x:%d y:%d\n",rdmCoord->x, rdmCoord->y );
	playMoove(jeu, rdmCoord, jeu->joueurCourant);

}

//Renvoie un couple de coordonnée random
Coord * randomCoord()
{
	int x = rand() % jeu->taille;
	//Le random se base sur time, on doit donc attendre pour avoir un nombre différent
	//sleep(1);
	int y = rand() % jeu->taille;
	return initCoord(x,y);
}

//Lance le jeu
void game(int argc, char *argv[])
{
	int largeur = 700;
	int hauteur = 700;
	bool loadPreviousGame = false;
	char* fileName;
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

		//si on veut charger un fichier
		if(strcmp(argv[i],"-load") == 0)
		{
			printf("Loading\n");
			loadPreviousGame = true;
			fileName = argv[i+1];
		}
		//
		if(strcmp(argv[i],"-taille") == 0)
		{
			taillePlateau = atoi(argv[i+1]);
		}
		//
		if(strcmp(argv[i],"-ia") == 0)
		{
			ia = true;
			//l'argument -ia est le seul argument sans paramètre, or on lie des couples de paramètre, on doit donc décrémenter i
			i--;
		}
	}

	init_win(largeur,hauteur, "Jeu de GO",246,254,185);

	//si on doit charger une partie existante
	if(loadPreviousGame)
	{
		printf("Starting load function \n");
		//filename = saveGame.sgf;
		loadPartyData(fileName, &jeu, &deroulementPartie);
		printf("Fin chargement, taille : %d\n", jeu->taille);

	}
	else
	{
		jeu = initJeu(taillePlateau);
		deroulementPartie = initDeroulementPartie(jeu);
	}

	event_loop();

}
