#include "../lib/gctop.h"

int main(int argc, char **argv){

	printERROL = -1;
	printERROC = -1;
	numVar = 0;
	int DEBUG = FALSE;
	// system("clear");

	if(argc == 3) DEBUG = atoi(argv[2]);

	//lê todas as palavras reservadas da linguagem C
	ReservedWords *rW = readRW("../arqComp/reservedWords.txt");

	//cria uma lista para armazenar os tokens
	TkList *tkl = createList();

	//calcula o tamanho do arquivo para armazena-lo em uma string
	int size = sizeArchive(argv[1]);

	//lê e armazena o código em uma string
	char *code = readCode(argv[1], size);
	
	//executa a verificação sobre o código, criando uma lista de tokens
	lexicalAnalyzer(argv[1], code, size, rW, tkl);
	// printf("Número de variaveis: %d\n", numVar);

	TableVars tableVars[numVar];
	tableVars[0].len = 0;

	if(DEBUG) printListTK(tkl);

	syntacticAnalyzer(tkl, tableVars, code, argv[1], size);
	printTableVars(tableVars);
	// printf("\n");
	
	freeList(tkl);
	free(rW); free(code);


	return 0;
}