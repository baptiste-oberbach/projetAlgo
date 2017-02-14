#ifndef sgfSaveHelper_h
#define sgfSaveHelper_h

void loadPartyData(char* fileName, Jeu** jeu, DeroulementPartie** deroulementPartie);
char* searchValueInBuffer(char* buffer, size_t buf_len, char* startDelimiter, char* endDelimiter);
void writePartyData(char* fileName, DeroulementPartie* deroulementPartie);
void writeFileBeginning(FILE* fp);
void writeFileFormat(FILE* fp);
void writeEncoding(FILE* fp);
void writeGameType(FILE* fp);
void writeBoardSize(FILE* fp, int taillePlateau);
void writeEventName(FILE* fp);
void writeRound(FILE* fp, int number);
void writeDateOfTheGame(FILE* fp);
void writeBlackPlayerName(FILE* fp, char* name);
void writeWhitePlayerName(FILE* fp, char* name);
void writeRule(FILE* fp);
void writeKomi(FILE* fp);
void writeResult(FILE* fp, char* resultStr);
void writeWriterName(FILE* fp);
void writeBlackMoove(FILE* fp, int xCoord, int yCoord);
void writeWhiteMoove(FILE* fp, int xCoord, int yCoord);
void writeEndFile(FILE* fp);



#endif /* sgfSaveHelper_h */
