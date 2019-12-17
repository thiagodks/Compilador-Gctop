#include "../lib/gctop.h"

struct element{
	Token tk;
	struct element *prox;
};

typedef struct element Elem;

TkList* createList(){

	TkList* tkl = (TkList*) malloc(sizeof(TkList));
	if(tkl != NULL) *tkl = NULL;
	return tkl;
}

void freeList(TkList* tkl){

	if(tkl != NULL){
		Elem* no;

		while((*tkl) != NULL){
			no = *tkl;
			*tkl = (*tkl)->prox;
			free(no);
		}
		free (tkl);
	}	
}

int sizeList(TkList* tkl){

	if(tkl == NULL) return 0;
	
	int cont = 0;
	Elem *no = *tkl;
	
	while(no != NULL){
		cont++;
		no = no->prox;
	}
	return cont;
}

int emptyList(TkList *tkl){

	if(tkl == NULL)
		return TRUE;
	
	return FALSE;
}

int insertToken(TkList *tkl, Token tk){
	
	if(tkl == NULL) return FALSE;
	
	Elem *no = (Elem*) malloc(sizeof(Elem));
	if(no ==  NULL) return FALSE;
	
	no->tk = tk;
	no->prox = NULL;
	
	if((*tkl) ==  NULL) *tkl = no;
	
	else{
		
		Elem *aux = *tkl;
		while(aux->prox != NULL) aux = aux->prox;

		aux->prox = no;
	}

	return TRUE;
}

void printTableVars(TableVars tableVars[]){

	printf("LEN: %d\n", tableVars[0].len);
	for(int i = 0; i < tableVars[0].len; i++){
		printf("\n\nName: %s\nType: %s\nValue: %s\nLine: %d\nColumn: %d\nInit: %d", tableVars[i].var.name, tableVars[i].var.type, tableVars[i].var.value, tableVars[i].var.line, tableVars[i].var.column, tableVars[i].var.init);
	}

}

int printListTK(TkList* tkl){

	if(tkl == NULL) return FALSE;
	
	int backg = 0;
	Elem *aux = *tkl;

	printf(BOLD_ON"\n\n                 ============= Lista de Tokens =============                \n"BOLD_OFF);
	printf("\u250F");
	for(int i = 0; i < 74; i++) printf("\u2501");
	printf("\u2513");
	printf(BOLD_ON"\n\u2503     TYPE    \u2502      ID      \u2502     LINE     \u2502    COLUMN    \u2502     VALUE     \u2503\n"BOLD_OFF);
	printf("\u2517");
	for(int i = 0; i < 74; i++) printf("\u2501");
	printf("\u251B\n");
	
	while(aux != NULL){
		if(backg%2 == 0){

			printf(BACKWHITE FOREBLACK"%12s \u2502 %12i \u2502 %12d \u2502 %12d \u2502 %12s"BACKNORMAL FORENORMAL_COLOR, aux->tk.type, aux->tk.id, aux->tk.line, aux->tk.column, aux->tk.value);
			printf("\n");
		}else{

			printf(" %12s \u2502 %12i \u2502 %12d \u2502 %12d \u2502 %12s\n", aux->tk.type, aux->tk.id, aux->tk.line, aux->tk.column, aux->tk.value);
		}
		aux = aux->prox;
		backg++;
	}
	return TRUE;
}