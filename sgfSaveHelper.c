#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#include "listeChainee.h"
#include "sgfSaveHelper.h"

//Espece de regex maison pour récuperer des infos du fichier
//buffer : liste du fichier
//buf_len : taille de ce buffer
//startDelimiter : chaine de caractere a trouver pour avoir les infos entre (Comme par exemple SZ[   )
//endDelimiter : chaine de caractere a trouver pour fermer ce qu'on cherche comme ici ]
//on retourne ce qu'on a retrouver entre les deux delimiters > exemple SZ[12] on va retourner "12"
char* searchValueInBuffer(char* buffer, size_t buf_len, char* startDelimiter, char* endDelimiter)
{
	char* finalResult = NULL;

	//Exemple : Ici le buffer vaux : 1]SZ[19]te
	char* indice;
	indice = strstr(buffer, startDelimiter);
	//strstr retourne un pointeur sur l'endroit ou on a trouvé la premiere occurence de la chaine
	if(indice != NULL)
	{
		//cherche a savoir la longueur de la prochaine chaine qu'il nous faut
		int position = buffer - indice; //get la position
		int substringLength = buf_len - position; //prends la longeur de notre substring
		char* subString = malloc(substringLength*sizeof(char*)); 
		//Par exemple ici on aura SZ[12345]BLALBLABLALBLABA 
		strncpy (subString, indice, substringLength); 

		//ici subString vaut SZ[19]te
		printf("substring = %s\n", subString);

		char* indiceEndDemiliter;
		indiceEndDemiliter = strstr(subString, endDelimiter);
		//On veut maintenant avoir seulement SZ[12345]
		if(indiceEndDemiliter != NULL)
		{
			char* pointeurToTheEndOfSubstring = subString + substringLength;
			int positionEnd = pointeurToTheEndOfSubstring - indiceEndDemiliter;
			int searchingPartLength = substringLength - positionEnd;
			char* searchingPart = malloc(searchingPartLength*sizeof(char*));
			strncpy (searchingPart, subString, searchingPartLength);

			printf("searchingPart %s\n", searchingPart);
			//ici searchingPart vaut SZ[19
			//On veut la chaine de caractere entre les delimiters, du coup on a plus 
			//qu'a pousser notre pointeur d'autant de caractere que compose le startDemiliter (pour l'exemple ici ca sera 3)
			int sizeToPush = strlen(startDelimiter);
			printf("Len de startDelimiter : %d\n", sizeToPush);
			int positionPushed = searchingPart - (searchingPart+sizeToPush); 
			printf("position pushed %d\n", positionPushed);
			printf("substringLength %d\n", substringLength);
			int finalResultLength = substringLength + positionPushed; 
			printf("finalResultLength %d\n", finalResultLength);

			finalResult = malloc(finalResultLength*sizeof(char*));
			//du coup on va pouvoir enfin avoir notre resultat (youhou)
			printf("Le malloc est fait\n");

			strncpy(finalResult, searchingPart+sizeToPush, finalResultLength);

			//ici avec notre exemple on aura finalResult = "19"
			printf("finalResult %s", finalResult);
		}
	
	}

	return finalResult;
}

//sert a load les données du fichier pour reprendre une partie
void loadPartyData(char* fileName, Jeu* jeu, DeroulementPartie* deroulementPartie)
{

	printf("Try to open save file : %s\n", fileName);
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL) {
		perror("Error opening file.");
	}
	else
	{
		//traitement du fichier, on va chercher les mots clés
		int sz = 0;

		char buffer[10];
	    char *input = 0;
	    size_t cur_len = 0;

	    //lecture ligne par ligne -> le buffer represente la ligne courante et input le fichier entier
	    while (fgets(buffer, sizeof(buffer), fp) != 0)
	    {
	        size_t buf_len = strlen(buffer);
	        char *extra = realloc(input, buf_len + cur_len + 1);
	        if (extra == 0)
	            break;
	        input = extra;
	        strcpy(input + cur_len, buffer);
	        cur_len += buf_len;

	        //recherche chaque clé dans le fichier pour pouvoir les traiter pour initaliser notre jeu

	        //check si on a deja remplit la size
	        if(sz == 0)
	        {
	        	char* size = searchValueInBuffer(buffer, buf_len, "SZ[", "]");
	        	printf("final size : %s\n", size);
	        	if(size != NULL)
	        	{
	        		jeu = initJeu(atoi(size));
	        		printf("jeu : taille %d\n", jeu->taille);
	        		printf("Start to initialize the party\n");
					deroulementPartie = initDeroulementPartie();
					printf("apres deroulement party\n");
					break;
	        	}
	        	
	        }
        	//sinon on recherche les autres valeurs dont on a besoin dans le fichier
	        
	        
	        
	        
	        

	    }
	    printf("Avant free input ?\n");
	    //printf("%s [%d]", input, (int)strlen(input));
	    free(input);

			/*init_win(largeur,hauteur, "Jeu de GO",246,254,185);
			jeu = initJeu(SZ);
			deroulementPartie = initDeroulementPartie();*/

		
		/*
		//parcours tous notre fichier et joue chaque coup
		Coord * coord = initCoord(posX,posY);


		playMoove(jeu,coord,jeu->joueurCourant);
		if(jeu->joueurCourant == BLANC)
		{
			jeu->lastCoordBlanc = coord;
			jeu->joueurCourant = NOIR;
		}
		else
		{
			jeu->lastCoordNoir = coord;
			jeu->joueurCourant = BLANC;
		}*/
	}
	printf("avant close fichier ?\n");
	fclose(fp);
	//draw la fenetre a la fin
	//draw_win();
}

void writePartyData(char* fileName, DeroulementPartie* deroulementPartie)
{

	printf("Try to create file : %s\n", fileName);
	//ab : Append; open or create file for writing at end-of-file.
	FILE *fp = fopen(fileName, "w");

	if(fp == NULL) {
		perror("Error opening file.");
	}
	else
	{
		printf("Try to write file\n");
		//write all informations step by step in file
		//start file
		writeFileBeginning(fp);
		//header
		writeFileFormat(fp);
		writeEncoding(fp);
		writeGameType(fp);
		writeBoardSize(fp, deroulementPartie->taillePlateau);
		writeEventName(fp);
		writeRound(fp, deroulementPartie->nbRound);
		writeDateOfTheGame(fp);
		writeBlackPlayerName(fp, "Joueur noir");
		writeWhitePlayerName(fp, "Joueur blanc");
		writeRule(fp);
		writeKomi(fp);
		//todo
		writeResult(fp, "");
		writeWriterName(fp);
		//end header

		//parcours tous les pions noirs | nbCoupPionNoir > nbCoupPionBlanc
		if(deroulementPartie->listePionNoir->first != NULL)
		{
			//Fait un pointeur pour qu'on puisse parcourir toute notre liste
			Noeud* noeudCourrantNoir = deroulementPartie->listePionNoir->first;
			Noeud* noeudCourrantBlanc = deroulementPartie->listePionBlanc->first;
			while(noeudCourrantNoir != NULL)
			{
				if(noeudCourrantNoir != NULL)
				{
					writeBlackMoove(fp, noeudCourrantNoir->pion->coord->x, noeudCourrantNoir->pion->coord->y);
					noeudCourrantNoir = noeudCourrantNoir->next;
				}
				
				if(noeudCourrantBlanc != NULL)
				{
					writeWhiteMoove(fp, noeudCourrantBlanc->pion->coord->x, noeudCourrantBlanc->pion->coord->y);
					noeudCourrantBlanc = noeudCourrantBlanc->next;
				}
				
			}
			free(noeudCourrantNoir);
			free(noeudCourrantBlanc);

		}

		writeEndFile(fp);

		printf("Votre partie a bien été sauvegardée !\n");
		
	}
	fclose(fp);

}

void writeFileBeginning(FILE* fp)
{
	fputs("(;\n", fp);
}

// FF: File format: version of SGF specification governing this SGF file. 
void writeFileFormat(FILE* fp)
{
	fputs("FF[4]", fp);
}

//Write the encoding like example file, but it's not in the official  
void writeEncoding(FILE* fp)
{
	fputs("CA[UTF-8]", fp);
}

//GM: Game: type of game represented by this SGF file. A property value of 1 refers to Go. 
void writeGameType(FILE* fp)
{
	fputs("GM[1]", fp);
}

//SZ: Size: size of the board, non square boards are supported. 
void writeBoardSize(FILE* fp, int taillePlateau)
{
	switch(taillePlateau)
	{
		case 19:
			fputs("SZ[19]\n", fp);
			break;
		case 13:
			fputs("SZ[13]\n", fp);
			break;
		case 9:
			fputs("SZ[9]\n", fp);
			break;
		default:
			perror("Error on retrieve the size of the chest");
			break;
	}

}

// EV: Event: name of the event (e.g. 58th Honinbo Title Match). 
void writeEventName(FILE* fp)
{
	fputs("EV[Awesome local party]\n", fp);
}

//RO: Round: round (e.g.: 5th game). 
void writeRound(FILE* fp, int number)
{
	fputs("RO[", fp);
	fprintf(fp, "%d", number);
	fputs("]\n", fp);

}

//DT: Date: date of the game. 
void writeDateOfTheGame(FILE* fp)
{
    time_t rawtime;
    struct tm* timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    // Must be static, otherwise won't work
    static char retval[20];
    strftime(retval, sizeof(retval), "%Y-%m-%d", timeinfo);
    fputs("DT[", fp);
    fprintf(fp, "%s", retval);
	fputs("]\n", fp);
}

//PB: Black Name: name of the black player. 
void writeBlackPlayerName(FILE* fp, char* name)
{
	fputs("PB[", fp);
    fprintf(fp, "%s", name);
    fputs("]\n", fp);
}

//PW: White Name: name of the white player. 
void writeWhitePlayerName(FILE* fp, char* name)
{
	fputs("PW[", fp);
    fprintf(fp, "%s", name);
    fputs("]\n", fp);
}

//RU: Rules: ruleset (e.g.: Japanese). 
void writeRule(FILE* fp)
{
	fputs("RU[Japanese]\n", fp);
}

//KM: Komi: komi. > http://senseis.xmp.net/?Komi
void writeKomi(FILE* fp)
{
	//default komi
	fputs("KM[6.5]\n", fp);
}

//RE: Result: result, usually in the format "B+R" (Black wins by resign) or "B+3.5" (black wins by 3.5 moku). 
void writeResult(FILE* fp, char* resultStr)
{
	fputs("RE[", fp);
    fprintf(fp, "%s", resultStr);
    fputs("]\n", fp);
}

//US: User: name of the person who created the SGF file. 
void writeWriterName(FILE* fp)
{
	fputs("US[System]\n", fp);
}

//For writing moove, we store value in letter so we add coord to ascii 'a' value to have the correct letter
//B: a move by Black at the location specified by the property value. 
void writeBlackMoove(FILE* fp, int xCoord, int yCoord)
{
	char xLetterCoord = 'a';
	char yLetterCoord = 'a';

	xLetterCoord += xCoord;
	yLetterCoord += yCoord;

	fputs("B[", fp);
	fprintf(fp, "%c", xLetterCoord);
	fprintf(fp, "%c", yLetterCoord);
	fputs("]", fp);

}

// W: a move by White at the location specified by the property value. 
void writeWhiteMoove(FILE* fp, int xCoord, int yCoord)
{
	char xLetterCoord = 'a';
	char yLetterCoord = 'a';

	xLetterCoord += xCoord;
	yLetterCoord += yCoord;

	fputs("W[", fp);
	fprintf(fp, "%c", xLetterCoord);
	fprintf(fp, "%c", yLetterCoord);
	fputs("]", fp);
}

void writeEndFile(FILE* fp)
{
	fputs("\n)", fp);
}
