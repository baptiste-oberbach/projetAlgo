#include <stdio.h>
#include <stdlib.h>
#include "go.h"
#include "dessine.h"


int main(int argc, char *argv[])
{
  printf("Début du programme de test du jeux de go \n");
  printf("Test de l'interface graphique \n");
  game(argc, argv);
  printf("Fin test de l'interface graphique \n");
}
