#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define BUFFSIZE 256 

///\par Función prompt.
/// Funcion encargada de imprimir el prompt de para el bash
void prompt(){
	char posicion[BUFFSIZE+1];
	char nombre[BUFFSIZE+1];
	char hostname[BUFFSIZE+1];
	//direccion actual de pwd
	getcwd(posicion,sizeof(posicion));
	//Se obtiene el nombre de Host
	gethostname(hostname, BUFFSIZE+1);
	//Se obtiene el nombre de usuario
	cuserid(nombre); 	
	strcat(nombre, "@");
    strcat(nombre, hostname);
    printf("\n%s:%s$ ",nombre, posicion);
	return;
}
