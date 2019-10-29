#include "../lib/gctop.h"

char separatorsFull[SP][2] = {"(", ")", "{", "}", "[", "]",",", ";", "=", "+", "-", "/", "*",
						  "+", "-", "/", "*", "%", "++", "--", "+=", "-=", "*=", "/=", 
						  ">=", "<=", ">", "<", "&&", "||", "&", "==", "!=", "!", "|", "@", " ", "\t", "\n"};

char separators[12][2] = {"(", ")", "{", "}", "[", "]",",", ";", "=", " ", "\t", "\n"};

char operatorsA[OP_A][3] = {"+", "-", "/", "*", "%", "++", "--", "+=", "-=", "*=", "/="};

char operatorsL[OP_L][3] = {">=", "<=", ">", "<", "&&", "||", "&", "==", "!=", "!"};

void clear(char word[256]){
	for(int i = 0; i < 256; word[i++] = '\0');
}

//verifica se o caracter é um separador
int isSeparator(char ch, char separatorsFull[SP][2], int size){

	char word[2];
	word[0] = ch; word[1] = '\0';

	for (int i = 0; i < size; i++){
		if(strcmp(separatorsFull[i], word) == 0) return TRUE;
	}
	return FALSE;
}

//verifica se é uma palavra reservada
int isReservedWord(char word[256], ReservedWords *rW){


	for(int i = 0; i < RW; i++){
		if(strcmp(word, rW[i].name) == 0){
			return TRUE;
		}
	}
	return FALSE;
}

//verifica se é um identificador
int isIdentifier(char word[256]){

	if(isalpha(word[0]) || word[0] == '_') return TRUE;

	if(isdigit(word[0])) return FALSE;

	return FALSE;	
}

//verifica se é um literal
int isLiteral(char word[256]){

	if(word[0] == '"' && word[strlen(word)-1] == '"' && strlen(word) >= 2) return TRUE;
	if(word[0] == 39 && word[strlen(word)-1] == 39 && strlen(word) == 3) return TRUE; 

	return FALSE;
}

//verifica se é um número
int isNumber(char word[256]){

	for(int i = 0; i < strlen(word); i++){
		if(!(isdigit(word[i]) || word[i] == '.'))
			return FALSE; 
	}
	if(!strlen(word)) return FALSE;

	return TRUE;
}

int getProxCh(char word[256], char comp[50][3], int size, char *code, int i){

	int op = 0;
	for(int i = 0; i < size; i++){
		if(strcmp(word, comp[i]) == 0) op = 1;
	}

	word[1] = code[i+1];
	word[2] = '\0';

	for(int i = 0; i < size; i++){
		if(strcmp(word, comp[i]) == 0) op = 2;
	}

	return op;
}

// verifica se é um operador aritmético
int isOperator_A(char ch, int i, char *code){

	char word[256];
	word[0] = ch; word[1] = '\0';

	int op = getProxCh(word, operatorsA, OP_A, code, i);

	if(op == 1) return 1;
	else if(op == 2) return 2;

	return FALSE;
}

//verifica se é um operador lógico
int isOperator_L(char ch, int i, char *code){

	char word[256];
	word[0] = ch; word[1] = '\0';

	int op = getProxCh(word, operatorsL, OP_L, code, i);

	if(op == 1) return 1;
	else if(op == 2) return 2;

	return FALSE;
}

int isComment(char ch, int *i, char *code){

	char word[256];
	int j;

	word[0] = ch; word[1] = code[(*i)+1]; word[2] = '\0';

	if(strcmp(word, "//") == 0){
		for(j = (*i); code[j] != '\n'; j++);
		(*i) = j;
		return TRUE;
	}
	else if(strcmp(word, "/*") == 0){
		for(j = (*i); code[j] != '*' || code[j+1] != '/'; j++);
		(*i) = j+1;
		return TRUE;
	}

	return FALSE;
}

//cria e inicializa os campos do token identificado
Token createToken(char type[256], char value[256], int line, int column, int id){

	Token newTK;
	strcpy(newTK.type, type);
	strcpy(newTK.value, value);
	newTK.id = id;
	newTK.line = line;
	newTK.column = column;

	return newTK;
}

//identifica caracteres ou palavras inválidas
int isError(char ch, char word[256]){

	if(ch < 0) return -1;
	if(ch == '@') return INVALID_CH;

	if(isdigit(word[0])){

		for(int i = 0; i < strlen(word); i++){
			if(isalpha(word[i])) return INVALID_SUFFIX;
		}
	}
	return FALSE;
}

//recupera a linha em que ocorreu o erro
char *getLineError(char *code, int current){

	char *line = (char*)malloc(1024*sizeof(char));
	int start = 0, end = 0, j = 0;
	for(int i = current; code[i] != '\n'; i--) start = i;
	for(int i = current; code[i] != '\n'; i++) end = i;
	for(int i = start; i < end+1; i++) line[j++] = code[i];
	line[j] = '\0';
	
	return line;
}

int operators(char *code, int i, char ch, char word[256], int line, int column, TkList *tkl){
	
	Token newTK;
	int op = isOperator_A(ch, i, code);
				
	if(op == 1) {
		word[0] = ch; word[1] = '\0';
		newTK = createToken("OPERATOR_A", word, line, column, OPERATOR_A);
		insertToken(tkl, newTK);

	//operador aritmetico de dois caractere
	}else if(op == 2){
		word[0] = ch; word[1] = code[i++]; word[2] = '\0';
		newTK = createToken("OPERATOR_A", word, line, column, OPERATOR_A);
		insertToken(tkl, newTK);
	}


	//verifica se o ch lido é um operador lógico e retorna 1, caso o operador tenha apenas 1 ch, ou 2 caso tenha 2 ch
	op = isOperator_L(ch, i, code);

	//operador logico de apenas um caractere
	if(op == 1) {
		word[0] = ch; word[1] = '\0';
		newTK = createToken("OPERATOR_L", word, line, column, OPERATOR_L);
		insertToken(tkl, newTK);
		
	//operador logico de dois caractere
	}else if(op == 2) {
		word[0] = ch; word[1] = code[i++]; word[2] = '\0';
		newTK = createToken("OPERATOR_L", word, line, column, OPERATOR_L);
		insertToken(tkl, newTK);
		
	}

	return i;
}

//executa a verificação sobre o código, criando uma lista de tokens
int lexicalAnalyzer(char *fileName, char *code, int size, ReservedWords *rW, TkList *tkl){

	char ch;
	char word[256] = "\0";
	int index = 0, line = 1, column = 1, pivo = 0, error = 0, comment = 0;

	//token para ir inserindo na lista de tokens
	Token newTK;

	for(int i = 0; code[i] != EOF; i++){

		//lendo caractere por caractere
		ch = code[i];

		//salva o pivo/sentinela para identificar o começo da palavra ou caractere
		if(!pivo) pivo = column;

		//cria a palavra com cada caractere lido
		word[index] = ch;
		index++; column++;

		if(ch == '\n'){ line++; column = 1; }

		//verifica se o caracter lido é um separador e se a palavra formada até agr n é um literal
		if(isSeparator(ch, separatorsFull, SP) && word[0] != '"'){

			word[index-1] = '\0';

			//verifica se a palavra encontrada é uma palavra reservada
			if(isReservedWord(word, rW)){
				newTK = createToken("RESERVED_W", word, line, pivo, RESERVED_W);
				insertToken(tkl, newTK);

			//verifica se a palavra encontrada é um identificador
			}else if(isIdentifier(word)){
				newTK = createToken("IDENTIFIER", word, line, pivo, IDENTIFIER);
				insertToken(tkl, newTK);
			
			//verifica se a palavra encontrada é um numero
			}else if(isNumber(word)){
				newTK = createToken("NUMBER", word, line, pivo, NUMBER);
				insertToken(tkl, newTK);
			
			}
			
			comment = isComment(ch, &i, code);

			if(!comment){

				// verifica se é algum operador (logico ou aritmético)
				i = operators(code, i, ch, word, line, column, tkl);
	
				//verifica se o separador lido está apenas na classe de separadores
				if(isSeparator(ch, separators, 9)){
					word[0] = ch; word[1] = '\0';
					newTK = createToken("SEPARATOR", word, line, column, SEPARATOR);
					insertToken(tkl, newTK);

				}
			}

			clear(word);
			pivo = index = 0;

		//verifica se a palavra lida é um literal
		}else if(isLiteral(word)){
			newTK = createToken("LITERAL", word, line, pivo, LITERAL);
			insertToken(tkl, newTK);
			clear(word);
			pivo = index = 0;

		}
		//por fim verifica se a palavra ou o caractere lido é um erro
		error = isError(ch, word);

		if(error == INVALID_CH) {
			word[0] = ch; word[1] = '\0';
			printError(fileName, line, pivo, word, getLineError(code, i), INVALID_CH);
		
		}else if(error == INVALID_SUFFIX) {
			printError(fileName, line, pivo, word, getLineError(code, i), INVALID_SUFFIX);
		
		}else if(error == -1){
			word[0] = ch; word[1] = code[i++]; word[2] = '\0';
			printError(fileName, line, pivo, word, getLineError(code, i), INVALID_CH);
			
		}
	}
				
	return TRUE;

}