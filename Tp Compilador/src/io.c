#include "../lib/gctop.h"

void printError(char *fileName, int line, int column, char *name, char *errorLine, int type){

	if(printERROL == line && printERROC == column) return;

	printERROL = line;
	printERROC = column;

	switch(type){

		case INVALID_CH:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Invalid character: '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;

		case INVALID_SUFFIX:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Invalid suffix in: '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;
	
		case INVALID_DEC:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Error in declaration: '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;

		case INVALID_ATR:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Error in attribution: '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;

		case INVALID_EXP:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Error in expression '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;

		case INVALID_ERRO:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Error in '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;
		case INVALID_DIR:
			printf(BOLD_ON"%s:%d:%d: "FORERED"error"FORENORMAL_COLOR BOLD_OFF": Error in directive '%s'\n", fileName, line, column, name);
			printf("%s\n\n", errorLine);
			break;
	}
	free(errorLine);

}

ReservedWords *readRW(char *fileName){

	int i = 0;
	char word[256];

	ReservedWords *rW = (ReservedWords*)malloc(RW*sizeof(ReservedWords));

	FILE *arqRW = fopen(fileName, "r");

	if(arqRW == NULL) return NULL;

	while(!feof(arqRW)){

		fscanf(arqRW, "%s\n", word);
		strcpy(rW[i].name, word);
		rW[i].id = i;
		i++;

	}

	fclose(arqRW);
	return rW;
}

int sizeArchive(char *codeName){

	FILE *arqCode = fopen(codeName, "r");
	if(arqCode == NULL) return FALSE;

	int size = 0;

	while(fgetc(arqCode) != EOF) size++;

	fclose(arqCode);
	return size;
}

char *readCode(char *codeName, int size){

	char *code = (char*) malloc((size+10)*sizeof(char));

	FILE *arqCode = fopen(codeName, "r");
	if(arqCode == NULL) return FALSE;

	int ch = 1;
	code[0] = '\n';
	while(!feof(arqCode)) code[ch++] = fgetc(arqCode);

	fclose(arqCode);
	return code;
}	

void printRW(ReservedWords *rW){

	printf("\nPalavras reservadas: \n");
	for(int j = 0; j < RW; j++){
		printf("%s - %d\n", rW[j].name, rW[j].id);
	}

}