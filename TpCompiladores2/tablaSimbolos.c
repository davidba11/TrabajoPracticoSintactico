#include "anlex.h"

/*********************HASH************************/
input *tabla;			//declarar la tabla de simbolos
int sizeTable=TAMHASH;		//utilizado para cuando se debe hacer rehash
int elemRehash=0;			//utilizado para cuando se debe hacer rehash

int h(const char *k, int m){ // K es el puntero id en aNlex.c
    unsigned h=0,g;
    int i;
    for (i=0;i<strlen(k);i++){
	h=(h << 4) + k[i];
	if ( (g=h&0xf0000000) ){
        	h=h^(g>>24);
		h=h^g;
        }
    }
    return h%m;
}

void initTabla(){	
    int i=0;
    tabla=(input*)malloc(sizeTable*sizeof(input));
    for(i=0;i<sizeTable;i++){
        strcpy(tabla[i].componenteLex,"-1");
    }
}

int esprimo(int n){
    int i;
    for(i=3;i*i<=n;i+=2)
	if (n%i==0)
            return 0;
    return 1;
}

int siguiente_primo(int n){
    if (n%2==0)
	n++;
    for (;!esprimo(n);n+=2);
    return n;
}

//en caso de que la tabla llegue al limite, duplicar el tamaño
void rehash(){
    input *vieja;
    int i;
    vieja=tabla;
    sizeTable=siguiente_primo(2*sizeTable);
    initTabla();
    for (i=0;i<sizeTable/2;i++){
	if(strcmp(vieja[i].componenteLex,"-1")!=0)
            insert(vieja[i]);
	}		
    free(vieja);
}

//insert una entrada en la tabla
void insert(input e){
    int pos;
    if (++elemRehash>=sizeTable/2)
	rehash();
    pos=h(e.lexema,sizeTable);
    while (strcmp(tabla[pos].componenteLex,"-1")!=0){ //tabla[pos].componenteLex!="-1"
        // aqui busca una posicion con -1 en la TS para agregar un elemento nuevo
	pos++;
	if (pos==sizeTable)
            pos=0;
    }
    tabla[pos]=e;

}

//busca una clave en la tabla, si no existe devuelve NULL, posicion en caso contrario
input* search(const char *clave){
    int pos;
    pos=h(clave,sizeTable);
    while(strcmp(tabla[pos].componenteLex,"-1")!=0 && strcmp(tabla[pos].lexema,clave)!=0 ){ //VEEEEER
	pos++;
	if (pos==sizeTable)
            pos=0;
    }
    return &tabla[pos];
}

void insertTablaSimbolos(const char *s, char *n){
    input e;
    strcpy(e.lexema,s);
    strcpy(e.componenteLex,n);
    insert(e);
}
// MODIFIQUE ESTO
void initTablaSimb(){
    insertTablaSimbolos(",","COMA");
    insertTablaSimbolos(":","DOS_PUNTOS");
    insertTablaSimbolos("[","L_CORCHETE");
    insertTablaSimbolos("]","R_CORCHETE");
    insertTablaSimbolos("{","L_LLAVE");
    insertTablaSimbolos("}","R_LLAVE");
    insertTablaSimbolos("true","PR_TRUE");
    insertTablaSimbolos("false","PR_FALSE");
    insertTablaSimbolos("null","PR_NULL");
    insertTablaSimbolos("\0","EOF");
}
