#include "anlex.h"

// Variables 
token t;                 // token global para recibir componentes del Analizador Lexico
FILE *archivo;           // Fuente JSON
char message[41];        // Mensaje de error.
short boolean_error=0;   // Bandera para mensajes.


//prototipos de funciones para cada NO TERMINAL
void json(void);
void element(void);
void array(void);
void array2(void);
void element_list(void);
void element_list2(void);
void object(void);
void object2(void);
void attribute_list(void);
void attribute_list2(void);
void attribute(void);
void attribute_name(void);
void attribute_value(void);

//implementacion de funciones

void error_message(char* message){
    // Función de mensaje de error.
    boolean_error=1;
    printf("Line %d:\t\nSYNTAX ERROR \n\t%s.\n",line(),message);	
}

void getToken(void) {
    //retorna la siguiente entrada de la fuente
    sigLex();
}

void match(char* c){
    //verifica si el token actual es igual al token a comparar
    //y luego llama al siguiente token de la fuente
    if (strcmp(t.componenteLex, c)==0)
        getToken(); 
    else
        error_message("Match error");
}

void json(){
    //se llama a la regla gramatical element
    element();

    //se llega al final del archivo
    match("EOF");
}

void element(){
    //element: regla de la gramatica

    if(strcmp(t.componenteLex,"L_LLAVE")==0){
        //si el token es una llave abierto, se espera que sea un objeto  
        object();
    }else if (strcmp(t.componenteLex,"L_CORCHETE")==0){
        //si el token es corchete abierto, se espera que sea una lista
        array();
    }else if(strcmp(t.componenteLex,"R_CORCHETE")==0 || strcmp(t.componenteLex,"R_LLAVE")==0||strcmp(t.componenteLex,"COMA")==0){
         //si el token es llave o corchete cerrada, o una coma, significa error
        sprintf(message,"Expected tokens \"{\" o \"[\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        //si no es ninguno de lo anterior, se lee el siguiente token
        getToken();
}

void array(){
    //array: regla de la gramatica
    if(strcmp(t.componenteLex,"L_CORCHETE")==0){
        //si el token es una corchete abierto, se hace match y se llama a array prima  
        match("L_CORCHETE");
        array2();
    }else if(strcmp(t.componenteLex,"R_CORCHETE")==0||strcmp(t.componenteLex,"R_LLAVE")==0||strcmp(t.componenteLex,"COMA")==0){
        //si el token actual es una llave o corchete cerrado, significa un error 
        sprintf(message,"Expected tokens \"[\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken();
}

void array2(){
    //si el token es un corchete cerrado
    if(strcmp(t.componenteLex,"R_CORCHETE")==0){
        match("R_CORCHETE"); //
    }else if(strcmp(t.componenteLex,"L_CORCHETE")==0||strcmp(t.componenteLex,"L_LLAVE")==0){
        //si el token es un corchete abierto
        element_list();
        match("R_CORCHETE");
    }else if(strcmp(t.componenteLex,"R_LLAVE")==0){
        //si el token es una llave cerrada
        sprintf(message,"Expected token \"[\" or \"]\" or \"{\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else
        getToken();
}

void element_list(){
    //si el token es un corchete abierto o una llave abierta
    if(strcmp(t.componenteLex,"L_CORCHETE")==0||strcmp(t.componenteLex,"L_LLAVE")==0){
        element();
        element_list2();    
    }else if(strcmp(t.componenteLex,"R_CORCHETE")==0){
        //si el token es un corchete cerrado
        sprintf(message,"Expected token \"[\" or \"{\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken();    
}

void element_list2(){
    //si el token es una coma
    if(strcmp(t.componenteLex,"COMA")==0){
        match("COMA");
        element();
        element_list2();
    }else if(strcmp(t.componenteLex,"R_CORCHETE")!=0){       
        //si el token es un corchete cerrado
        getToken(); 
    }
}

void object(){
    //si el token es una llave abierta
    if(strcmp(t.componenteLex,"L_LLAVE")==0){
        match("L_LLAVE");
        object2(); 
    }else if(strcmp(t.componenteLex,"R_CORCHETE")==0||strcmp(t.componenteLex,"R_LLAVE")==0||strcmp(t.componenteLex,"COMA")==0){
        //si el token es un corchete cerrado, una llave cerrada o una coma, significa error
        sprintf(message,"Expected tokens \"{\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken();
}

void object2(){
    //si el token es una llave cerrada
    if(strcmp(t.componenteLex,"R_LLAVE")==0){
        match("R_LLAVE");   
    }else if(strcmp(t.componenteLex,"LITERAL_CADENA")==0){
        //si el token es un string
        attribute_list(); 
        match("R_LLAVE");
    }else if(strcmp(t.componenteLex,"R_CORCHETE")==0||strcmp(t.componenteLex,"COMA")==0){
        //si el token es un corchete cerrado o una coma
        sprintf(message,"Expected tokens \"}\" or \"string\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken();     
}

void attribute_list(){
    //si el token es un string
    if(strcmp(t.componenteLex,"LITERAL_CADENA")==0){
        attribute();
        attribute_list2(); 
    }else if(strcmp(t.componenteLex,"R_LLAVE")==0){
        //si el token es una llave cerradas
        sprintf(message,"Expected tokens \"string\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken(); 
}

void attribute(){
    //si el token es un string
    if(strcmp(t.componenteLex,"LITERAL_CADENA")==0){
        //puntero auxiliar para extraer las commillas.
        //char* aux=t.p->lexema;
        //aux++;
        //aux[strlen(aux)-1] = '\0';
        attribute_name();
        match("DOS_PUNTOS");
        attribute_value(); 
    }else if(strcmp(t.componenteLex,"R_LLAVE")==0||strcmp(t.componenteLex,"COMA")==0){
        sprintf(message,"Expected tokens \"string\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken();    
}

void attribute_list2(){
    //si el token es una coma
    if(strcmp(t.componenteLex,"COMA")==0){
        match("COMA");
        attribute();
        attribute_list2();
    }else if(strcmp(t.componenteLex,"R_LLAVE")!=0){ 
        //si el token es una llave cerrada
        getToken(); 
    }       
}

void attribute_name(){
    //se espera un string
    if(strcmp(t.componenteLex,"LITERAL_CADENA")==0){
        match("LITERAL_CADENA");
    }else
        getToken();
}

void attribute_value(){
    //se espera una llave abierta
    if(strcmp(t.componenteLex,"L_LLAVE")==0||strcmp(t.componenteLex,"L_CORCHETE")==0){    
        element();
    }//se espera una cadena
    else if(strcmp(t.componenteLex,"LITERAL_CADENA")==0){
        match("LITERAL_CADENA");
    }//se espera un numero
    else if(strcmp(t.componenteLex,"LITERAL_NUM")==0){
        match("LITERAL_NUM");
    }//se espera un TRUE
    else if(strcmp(t.componenteLex,"PR_TRUE")==0){        
        match("PR_TRUE");
    }//se espera un FALSE
    else if(strcmp(t.componenteLex,"PR_FALSE")==0){        
        match("PR_FALSE");
    }//se espera un NULL
    else if(strcmp(t.componenteLex,"PR_NULL")==0){        
        match("PR_NULL");
    }//caso contrario
    else if(strcmp(t.componenteLex,"R_LLAVE")==0||strcmp(t.componenteLex,"COMA")==0||strcmp(t.componenteLex,"DOS_PUNTOS")==0){
        sprintf(message,"Expected tokens \"{\" or \"[\" or \"string\" or \"number\" or \"true\" or \"false\" o r\"null\" not like that \"%s\"", t.p->lexema);
        error_message(message);
    }else 
        getToken(); 
}

void inicio(void){
    //se llama al simbolo inicial de la gramatica
    json(); 
    if(strcmp(t.componenteLex,"EOF")!=0) error_message("Not like that END OF FILE");
}

void parser(){
    while (strcmp(t.componenteLex,"EOF")!=0){
        getToken();
        inicio();
    }   
}

int main(int argc,char* args[]){
    
    //se inicializa la tabla de simbolos
    initTabla();

    //se carga la tabla de simbolos
    initTablaSimb();
     
    if(argc > 1){

        //se verifica la direccion y nombre del archivo    
    	if ( !( archivo=fopen(args[1],"rt") ) ){
                printf("File not found\n");
                exit(1);
    	}

        //se recorre el archivo
    	while (strcmp(t.componenteLex,"EOF")!=0){
                parser();
                //si no se encontraron errores en la fuente
                if(boolean_error==0)
                    printf("Correct font syntax.\n");
    	}

        //se cierra el archivo
    	fclose(archivo);

    }else{//no se paso el nombre del archivo fuente
    	printf("No file name\n");
    	exit(1);
    }

    return 0;
}
