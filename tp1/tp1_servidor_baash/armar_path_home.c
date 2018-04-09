#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "parsear_entrada.h"
#define BUFFSIZE 256


///\par Función armar_path_home.
///Si el path empieza con "~/" esta funcion se encarga de pasarlo a un PATH absoluto
void armar_path_home(char *camino,char *prefix){
	int i1=0,i=0;
    char * camino_parseado[BUFFSIZE+1];
    char nombre[BUFFSIZE+1];

    strcat( prefix, "/home/" );
    cuserid(nombre);
    strcat( prefix, nombre);
	i1=parsear_entrada(camino,camino_parseado,"~/");
    for (i = 0; i < i1; ++i){
        strcat( prefix, "/" );		//se concatena las barras a prefix 
        strcat( prefix, camino_parseado[i] ); //luego se completa para tener   
    }								//el PATH absoluto
	return;
}
