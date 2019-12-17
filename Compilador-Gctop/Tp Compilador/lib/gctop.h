#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "colors.h"

#define TRUE 1
#define FALSE 0

#define RW 34
#define SP 39
#define OP_A 11
#define OP_L 10

#define RESERVED_W 100
#define SEPARATOR 101
#define IDENTIFIER 102
#define LITERAL 103
#define NUMBER 104
#define OPERATOR_A 105
#define OPERATOR_L 106

#define INVALID_CH 107
#define INVALID_SUFFIX 108
#define INVALID_DEC 109
#define INVALID_ATR 110
#define INVALID_EXP 111
#define INVALID_ERRO 112
#define INVALID_DIR 113

struct token{
	char type[256];
	char value[256];
	int id;
	int line, column;
};

struct reservedWord{
	char name[256];
	int id;
};

struct variable{
	char name[256];
	char type[256];
	char value[256];
	int line, column;
	int init;
	int used;
};

typedef struct variable Variable;
typedef struct token Token;
typedef struct element *TkList;
typedef struct reservedWord ReservedWords;

struct tableVariables{
	Variable var;
	int len;
};

typedef struct tableVariables TableVars;

/* ================ GCTOP ================ */ 

int lexicalAnalyzer(char *fileName, char *code, int size, ReservedWords *rW, TkList *tkl);

int syntacticAnalyzer(TkList *tkl, TableVars tables[], char *code, char *fileName, int size);

void clear(char word[256]);

int isSeparator(char ch, char separatorsFull[SP][2], int size);

int isReservedWord(char word[256], ReservedWords *rW);

int isIdentifier(char word[256]);

int isLiteral(char word[256]);

int isNumber(char word[256]);

int isOperator_A(char ch, int i, char *code);

int isOperator_L(char ch, int i, char *code);

Token createToken(char type[256], char value[256], int line, int column, int id);

int isError(char ch, char word[256]);

char *getLineError(char *code, int current);

/* ================ TOKENS LIST ================ */ 

TkList* createList();

void freeList(TkList* tkl);

int insertToken(TkList *tkl, Token tk);

int sizeList(TkList* tkl);

int emptylist(TkList *tkl);


/* ================ IO ================ */ 

ReservedWords *readRW(char *fileName);

char *readCode(char *codeName, int size);

int sizeArchive(char *codeName);

void printRW(ReservedWords *rW);

int printListTK(TkList* tkl);

void printTableVars();

void printError(char *fileName, int line, int column, char *name, char *errorLine, int type);

int sizeArchive(char *codeName);

char *readCode(char *codeName, int size);

int printERROL;
int printERROC;
int numVar;