#include "../lib/gctop.h"

struct element{
	Token tk;
	struct element *prox;
};

typedef struct element Elem;

void printToken(Token token){

	printf(" %12s \u2502 %12i \u2502 %12d \u2502 %12d \u2502 %12s\n", token.type, token.id, token.line, token.column, token.value);
	getchar();
}

Elem *gotoCH(Elem *currentTk, char ch){

	while(currentTk->tk.value[0] != ch)
		currentTk = currentTk->prox;

	return currentTk;
}

Elem *SkipCH(Elem *currentTk, char ch){

	while(currentTk->tk.value[0] == ch){
		if(currentTk->prox == NULL) break;
		currentTk = currentTk->prox;
	}

	return currentTk;
}

int getPosition(char *code, int size, int line){

	int cont = 1;

	for(int i = 0; i < size; i++){
		if(code[i] == '\n') cont++;
		if(cont == line) {
			return i+1;
		
		}
	}
	return -1;
}

int isIdentf(char type[256]){

	if(strcmp(type,"IDENTIFIER") == 0) return TRUE;
	return FALSE;
}

int isSep(char type[256]){
	
	if(strcmp(type,"SEPARATOR") == 0) return TRUE;
	return FALSE;
}

int isNum(char type[256]){
	
	if(strcmp(type,"NUMBER") == 0) return TRUE;
	return FALSE;
}

int isOpA(char type[256]){
	
	if(strcmp(type,"OPERATOR_A") == 0) return TRUE;
	return FALSE;
}

int isOpL(char type[256]){
	
	if(strcmp(type,"OPERATOR_L") == 0) return TRUE;
	return FALSE;
}

int isType(char value[256]){

	if(strcmp(value,"int") == 0) return TRUE;
	if(strcmp(value,"float") == 0) return TRUE;
	if(strcmp(value,"char") == 0) return TRUE;
	if(strcmp(value,"void") == 0) return TRUE;
	return FALSE;
}

int isWhile(char value[256]){

	if(strcmp(value,"while") == 0) return TRUE;
	return FALSE;
}

int isFor(char value[256]){

	if(strcmp(value,"for") == 0) return TRUE;
	return FALSE;
}

int isIf(char value[256]){

	if(strcmp(value,"if") == 0) return TRUE;
	return FALSE;
}

int isElse(char value[256]){

	if(strcmp(value,"else") == 0) return TRUE;
	return FALSE;
}

int isElseIf(char value1[256], char value2[256]){

	if((strcmp(value1,"else") == 0) && (strcmp(value2,"if") == 0)) return TRUE;
	return FALSE;
}

int isDirectiveIn(char value[256]){

	if(strcmp(value,"#include") == 0) return TRUE;
	return FALSE;
}

int isDirectiveDef(char value[256]){

	if(strcmp(value,"#define") == 0) return TRUE;
	return FALSE;
}

Elem *checkStatement(Elem *currentTk, int *erro, Token *erroStat, char *fileName, char *code, int size);
Elem *checkListArg(Elem *currentTk, int *erro, Token *erroList, char *fileName, char *code, int size){

	while(currentTk->tk.value[0] != ')'){
		if(isType(currentTk->tk.value)){
			currentTk = currentTk->prox;
			if(isIdentf(currentTk->tk.type)){
				currentTk = currentTk->prox;
				if(currentTk->tk.value[0] == ',')
					currentTk = currentTk->prox;
				else if(currentTk->tk.value[0] != ')'){
					*erroList = currentTk->tk;
					currentTk = currentTk->prox;
					printError(fileName, erroList->line, erroList->column, erroList->value, getLineError(code, getPosition(code, size, erroList->line)), INVALID_DEC);
				}	
			}else{
				*erroList = currentTk->tk;
				currentTk = currentTk->prox;
				printError(fileName, erroList->line, erroList->column, erroList->value, getLineError(code, getPosition(code, size, erroList->line)), INVALID_DEC);
				//erro
			}
		}else{
			*erroList = currentTk->tk;
			currentTk = currentTk->prox;
			printError(fileName, erroList->line, erroList->column, erroList->value, getLineError(code, getPosition(code, size, erroList->line)), INVALID_DEC);
		}
	}
	*erro = FALSE;
	return currentTk;
}

Elem *checkCode(Elem *currentTk, int *erro, Token *erroCode, char *fileName, char *code, int size);
Elem *checkFunc(Elem *currentTk, int *erro, Token *erroFunc, char *fileName, char *code, int size){

	while(TRUE){

		if(isType(currentTk->tk.value)){
			currentTk = checkListArg(currentTk, erro, erroFunc, fileName, code, size);
		}

		if(currentTk->tk.value[0] == ')' && currentTk->prox->tk.value[0] == '{'){
			currentTk = currentTk->prox;
			if(currentTk->tk.value[0] == '{'){
				currentTk = checkCode(currentTk, erro, erroFunc, fileName, code, size);
				if(*erro){
					printError(fileName, erroFunc->line, erroFunc->column, erroFunc->value, getLineError(code, getPosition(code, size, erroFunc->line)), INVALID_DEC);
					*erro = FALSE;
				}
				break;
			}
		}else{
			printf("ERrro\n");
			getchar();
		}

	}
	*erro = FALSE;
	return currentTk;
}

Elem *checkDeclaration(Elem *currentTk, int *erro, Token *erroDec, char *fileName, char *code, int size){

	while(currentTk->tk.value[0] != ';'){

		if(isIdentf(currentTk->tk.type)){
			
			currentTk = currentTk->prox;
			if(currentTk->tk.value[0] == ';'){
				*erro = FALSE;
				return currentTk;
			
			}else if(currentTk->tk.value[0] == '('){
				currentTk = currentTk->prox;
				currentTk = checkFunc(currentTk, erro, erroDec, fileName, code, size);
				break;
			}

			while(currentTk->tk.value[0] != ';'){
				if(currentTk->tk.value[0] == ','){
					currentTk = currentTk->prox;
					if(isIdentf(currentTk->tk.type))
						currentTk = currentTk->prox;
				
				}else{
					*erro = TRUE;
					*erroDec = currentTk->tk;
					return gotoCH(currentTk, ';');
				}
			}
		}
	}
	*erro = FALSE;
	return currentTk;
}

Elem *checkAttribution(Elem *currentTk, int *erro, Token *erroAtr){

	while(currentTk->tk.value[0] != ';'){

		if(currentTk->tk.value[0] == '='){
			
			currentTk = currentTk->prox;

			while(currentTk->tk.value[0] != ';'){

				if(isIdentf(currentTk->tk.type) || isNum(currentTk->tk.type)){
					currentTk = currentTk->prox;
					if(isOpA(currentTk->tk.type))
						currentTk = currentTk->prox;
					if(currentTk->tk.value[0] == ')')break;
				
				}else{
					*erro = TRUE;
					*erroAtr = currentTk->tk;
					return gotoCH(currentTk, ';');
				}
			}
			break;

		}else{
			*erro = TRUE;
			*erroAtr = currentTk->tk;
			return gotoCH(currentTk, ';');
		}
	}

	*erro = FALSE;
	return currentTk;
}

Elem *checkExpression(Elem *currentTk, int *erro, Token *erroExp, char stop){

	while(currentTk->tk.value[0] != stop){

		if(isIdentf(currentTk->tk.type) || isNum(currentTk->tk.type)){
			currentTk = currentTk->prox;
			if(isOpA(currentTk->tk.type) || isOpL(currentTk->tk.type))
				currentTk = currentTk->prox;
			else if(stop != ';' && currentTk->tk.value[0] != ')'){
				*erro = TRUE;
				*erroExp = currentTk->tk;
				return gotoCH(currentTk, '{');
			
			}else if(currentTk->tk.value[0] == ';' || currentTk->tk.value[0] == ')') break;
		
		}else if(currentTk->tk.value[0] != ';'){
			*erro = TRUE;
			*erroExp = currentTk->tk;
			return gotoCH(currentTk, ';');
		}
	}
	currentTk = currentTk->prox;
	*erro = FALSE;
	return currentTk;
}

Elem *checkWhile(Elem *currentTk, int *erro, Token *erroLoop, char *fileName, char *code, int size);
Elem *checkFor(Elem *currentTk, int *erro, Token *erroLoop, char *fileName, char *code, int size);

Elem *checkStatement(Elem *currentTk, int *erro, Token *erroStat, char *fileName, char *code, int size){

	if(isType(currentTk->tk.value)){
		currentTk = currentTk->prox;
		currentTk = checkDeclaration(currentTk, erro, erroStat, fileName, code, size);
		if(currentTk->tk.value[0] == '}') currentTk->tk.value[0] = 'x';
		if(*erro) return currentTk;
		
	}else if(isIdentf(currentTk->tk.type)){
		currentTk = currentTk->prox;
		currentTk = checkAttribution(currentTk, erro, erroStat);
		if(currentTk->tk.value[0] == '}') currentTk->tk.value[0] = 'x';
		if(*erro) return currentTk;
	
	}else if(isWhile(currentTk->tk.value) || isIf(currentTk->tk.value)){
		currentTk = currentTk->prox;
		currentTk = checkWhile(currentTk, erro, erroStat, fileName, code, size);
		currentTk->tk.value[0] = 'x';
		if(erro) return currentTk;
	
	}else if(isFor(currentTk->tk.value)){
		currentTk = currentTk->prox;
		currentTk = checkFor(currentTk, erro, erroStat, fileName, code, size);
		currentTk->tk.value[0] = 'x';
		if(erro) return currentTk;

	}else{
		// currentTk->tk.value[0] = 'x';
		// printf("(CAIU AQUI CERTEZAAAAAA)\n");
		// currentTk = SkipCH(currentTk, '}');
	}

	*erro = FALSE;
	return currentTk;
}

Elem *checkInclude(Elem *currentTk, int *erro, Token *erroIn){

	if(currentTk->tk.value[0] == '<'){
		currentTk = currentTk->prox;
		if(isIdentf(currentTk->tk.type)){
			currentTk = currentTk->prox;
			if(currentTk->tk.value[0] == '>'){
				*erro = FALSE;
				return currentTk;
			}else{
				*erro = TRUE;
				*erroIn = currentTk->tk;
				return gotoCH(currentTk, ';');
			}

		}else{
			*erro = TRUE;
			*erroIn = currentTk->tk;
			return gotoCH(currentTk, ';');
		}
	}else{
		*erro = TRUE;
		*erroIn = currentTk->tk;
		return gotoCH(currentTk, ';');
	}

	*erro = FALSE;
	return currentTk;
}

Elem *checkDefine(Elem *currentTk, int *erro, Token *erroDef){

	if(isIdentf(currentTk->tk.type)){
		currentTk = currentTk->prox;
		if(isNum(currentTk->tk.type)){
			*erro = FALSE;
			return currentTk;

		}else{
			*erro = TRUE;
			*erroDef = currentTk->tk;
			return gotoCH(currentTk, ';');
		}
	}else{
		*erro = TRUE;
		*erroDef = currentTk->tk;
		return gotoCH(currentTk, ';');
	}

	*erro = FALSE;
	return currentTk;
}

Elem *checkCode(Elem *currentTk, int *erro, Token *erroCode, char *fileName, char *code, int size){

	if(currentTk->tk.value[0] == '{'){
		while(currentTk->tk.value[0] != '}' && currentTk->prox != NULL){
			currentTk = currentTk->prox;
			if(currentTk == NULL) break;
			currentTk = checkStatement(currentTk, erro, erroCode, fileName, code, size);
			if(*erro) printError(fileName, erroCode->line, erroCode->column, erroCode->value, getLineError(code, getPosition(code, size, erroCode->line)), INVALID_DEC);
			*erro = FALSE;
		}
		if(currentTk->tk.value[0] != '}' && currentTk->prox == NULL){
			printError(fileName, currentTk->tk.line, currentTk->tk.column, "Expected '}'", getLineError(code, getPosition(code, size, currentTk->tk.line)), INVALID_ERRO);
		}/*else{
			printf("diferente\n");
			printToken(currentTk->tk);
		}*/
	}else{
		currentTk->tk.line -= 1;
		currentTk->tk.column = 1;
		printError(fileName, currentTk->tk.line, currentTk->tk.column, "Expected '{'", getLineError(code, getPosition(code, size, currentTk->tk.line)), INVALID_DEC);
	}
	*erro = FALSE;
	return currentTk;
}

Elem *checkElse(Elem *currentTk, int *erro, Token *erroElse, char *fileName, char *code, int size){

	return checkCode(currentTk, erro, erroElse, fileName, code, size);
}

Elem *checkWhile(Elem *currentTk, int *erro, Token *erroLoop, char *fileName, char *code, int size){

	while(TRUE){

		if(currentTk->tk.value[0] == '('){
			
			currentTk = currentTk->prox;
			currentTk = checkExpression(currentTk, erro, erroLoop, ')');
			if(*erro){
				printError(fileName, erroLoop->line, erroLoop->column, erroLoop->value, getLineError(code, getPosition(code, size, erroLoop->line)), INVALID_EXP);
				*erro = FALSE;
			} 

			currentTk = checkCode(currentTk, erro, erroLoop, fileName, code, size);
			break;
		
		}else{
			*erro = TRUE;
			*erroLoop = currentTk->tk;
			return gotoCH(currentTk, ';');
		}
	}
	*erro = FALSE;
	return currentTk;

}

Elem *checkFor(Elem *currentTk, int *erro, Token *erroLoop, char *fileName, char *code, int size){

	while(TRUE){

		if(currentTk->tk.value[0] == '('){
			
			currentTk = currentTk->prox;
			if(isType(currentTk->tk.value)){
				currentTk = currentTk->prox;
				currentTk = checkDeclaration(currentTk, erro, erroLoop, fileName, code, size);
				if(*erro) printError(fileName, erroLoop->line, erroLoop->column, erroLoop->value, getLineError(code, getPosition(code, size, erroLoop->line)), INVALID_DEC);
				*erro = FALSE;

			}else if(isIdentf(currentTk->tk.type)){
				currentTk = currentTk->prox;
				currentTk = checkAttribution(currentTk, erro, erroLoop);
				if(*erro) printError(fileName, erroLoop->line, erroLoop->column, erroLoop->value, getLineError(code, getPosition(code, size, erroLoop->line)), INVALID_ATR);
				*erro = FALSE;
			
			}else{
				*erro = TRUE;
				*erroLoop = currentTk->tk;
				printError(fileName, erroLoop->line, erroLoop->column, erroLoop->value, getLineError(code, getPosition(code, size, erroLoop->line)), INVALID_ERRO);
				*erro = FALSE;
				currentTk = gotoCH(currentTk, ';');
			}

			if(currentTk->tk.value[0] == ';'){
				currentTk = currentTk->prox;
				currentTk = checkExpression(currentTk, erro, erroLoop, ';');
				if(*erro){
					printError(fileName, erroLoop->line, erroLoop->column, erroLoop->value, getLineError(code, getPosition(code, size, erroLoop->line)), INVALID_EXP);
					currentTk = currentTk->prox;
				} 
				*erro = FALSE;
			}

			if(isIdentf(currentTk->tk.type)){
				currentTk = currentTk->prox;
				// currentTk = currentTk->prox;
				currentTk = checkAttribution(currentTk, erro, erroLoop);
				if(*erro) printError(fileName, erroLoop->line, erroLoop->column, erroLoop->value, getLineError(code, getPosition(code, size, erroLoop->line)), INVALID_ATR);
				*erro = FALSE;
				currentTk = currentTk->prox;
			}

			currentTk = checkCode(currentTk, erro, erroLoop, fileName, code, size);
			break;
		
		}else{
			*erro = TRUE;
			*erroLoop = currentTk->tk;
			return gotoCH(currentTk, ';');
		}
	}
	*erro = FALSE;
	return currentTk;

}

int syntacticAnalyzer(TkList *tkl, char *code, char *fileName, int size){

	Elem *tkl_aux = *tkl;
	int erro = FALSE;

	while(tkl_aux != NULL){

		if(isType(tkl_aux->tk.value)){

			Token erroDec;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkDeclaration(tkl_aux, &erro, &erroDec, fileName, code, size);
			if(erro) 
				printError(fileName, erroDec.line, erroDec.column, erroDec.value, getLineError(code, getPosition(code, size, erroDec.line)), INVALID_DEC);
			erro = FALSE;
		
		}else if(isIdentf(tkl_aux->tk.type)){

			Token erroAtr;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkAttribution(tkl_aux, &erro, &erroAtr);
			if(erro) 
				printError(fileName, erroAtr.line, erroAtr.column, erroAtr.value, getLineError(code, getPosition(code, size, erroAtr.line)), INVALID_ATR);
			erro = FALSE;

		}else if(isWhile(tkl_aux->tk.value) || isIf(tkl_aux->tk.value) || isElseIf(tkl_aux->tk.value, tkl_aux->prox->tk.value)){

			if(isElseIf(tkl_aux->tk.value, tkl_aux->prox->tk.value)) tkl_aux = tkl_aux->prox;

			Token erroWhile;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkWhile(tkl_aux, &erro, &erroWhile, fileName, code, size);
			if(erro){
				printError(fileName, erroWhile.line, erroWhile.column, erroWhile.value, getLineError(code, getPosition(code, size, erroWhile.line)), INVALID_ERRO);
			}
			erro = FALSE;

		}else if(isFor(tkl_aux->tk.value)){

			Token erroFor;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkFor(tkl_aux, &erro, &erroFor, fileName, code, size);
			if(erro){
				printError(fileName, erroFor.line, erroFor.column, erroFor.value, getLineError(code, getPosition(code, size, erroFor.line)), INVALID_ERRO);
			}
			erro = FALSE;

		}else if(isElse(tkl_aux->tk.value)){
			
			Token erroElse;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkElse(tkl_aux, &erro, &erroElse, fileName, code, size);
			if(erro){
				printError(fileName, erroElse.line, erroElse.column, erroElse.value, getLineError(code, getPosition(code, size, erroElse.line)), INVALID_ERRO);
			}
			erro = FALSE;


		}else if(isDirectiveIn(tkl_aux->tk.value)){

			Token erroIn;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkInclude(tkl_aux, &erro, &erroIn);
			if(erro){
				printError(fileName, erroIn.line, erroIn.column, erroIn.value, getLineError(code, getPosition(code, size, erroIn.line)), INVALID_DIR);
			}
			erro = FALSE;

		}else if(isDirectiveDef(tkl_aux->tk.value)){
			Token erroDef;
			tkl_aux = tkl_aux->prox;
			tkl_aux = checkDefine(tkl_aux, &erro, &erroDef);
			if(erro){
				printError(fileName, erroDef.line, erroDef.column, erroDef.value, getLineError(code, getPosition(code, size, erroDef.line)), INVALID_DIR);
			}
			erro = FALSE;

		}

		tkl_aux = tkl_aux->prox;
	}

	return TRUE;
}