#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "listeChainee.h"
#include "sgfSaveHelper.h"

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

		printf("GG WWPPPPP\n");
		
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
