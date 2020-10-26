#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

#define TAMBUFF 5
#define TAMCMLEX 50
#define TAMLEX 50
#define TAMHASH 101

typedef struct input{
    char componenteLex[TAMCMLEX];    //definir los campos de 1 entrada de la tabla de simbolos
    char lexema[TAMCMLEX];	
    struct input *tipo;  // null puede representar variable no declarada	
} input;

typedef struct {
    char componenteLex[TAMCMLEX];
    input *p;
} token;

void insert(input e);
input* search(const char *key);
void initTabla();
void initTablaSimb();
void sigLex();
int line();
