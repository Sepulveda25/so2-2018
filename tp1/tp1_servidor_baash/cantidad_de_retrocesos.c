#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "parsear_entrada.h"
#define BUFFSIZE 256 

///\par Función cantidad_de_retrocesos.
/// Se cuentan la cantidad de retrocesos "../" y se concatenan el path ingresado con el pwd para armar el path absoluto
void cantidad_de_retrocesos(char *camino,char *prefix){
	
	int i1=0,i2=0; // indice para el parceador
	int i=0;
    char * camino_parseado[BUFFSIZE+1];
    char * PWD_parseado[BUFFSIZE+1];
    char posicion[BUFFSIZE+1];
   	int cantdad_retrocesos=0;
    
    //se parsea el path ingresado
 	i1=parsear_entrada(camino,camino_parseado,"/");


	//se parsea el PWD 
	getcwd(posicion,sizeof(posicion)); //direccion actual de pwd
	i2=parsear_entrada(posicion,PWD_parseado,"/");

	// se cuentan la cantidad de retrocesos en base al numero de "../" consecutivos
	while(camino_parseado[i]!=NULL){
		// printf("\n%s",camino_parseado[i] );
		if((*(camino_parseado[i])=='.')&&(*(camino_parseado[i]+1)=='.')){
			cantdad_retrocesos++;
		}
		i++;
	}

	//se controla que el path no vaya mas atras de lo que se puede
	if(cantdad_retrocesos>i2){
		printf("\n La cantidad de retrocesos supera el PATH");
		return;
	}
	
	//se comienza concatenando los elementos de PWD_parseado a prefix
    for (i = 0; i < (i2-cantdad_retrocesos); ++i){
        strcat( prefix, "/" );		
        strcat( prefix, PWD_parseado[i] );  
    }								
	
	//luego se sigue concatenando el path relativo (que se ingreso) a prefix 
	//para formar finalmente el path absoluto

	for (i = cantdad_retrocesos; i < i1; ++i){
        strcat( prefix, "/" );		
        strcat( prefix, camino_parseado[i] );  
    }	
    
	return;
}
