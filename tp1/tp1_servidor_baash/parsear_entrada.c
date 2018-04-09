#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM 1024

/*!
  \param entrada es la cadena que se quiere parsear.
  \param entrada_parseada[] es donde se va almacenando la cadena ya parseada.
  \param caracteres son los caracteres que se quieren quitar de la cadena
   \return Devuelve la longitud de entrada_parseada.
*/
///\par Función parsear_entrada.
/// Esta funcion quita la los simbolos (que se le pasan caracteres). Es util para transformar una cadena en una lista de cadenas
int parsear_entrada(char *entrada,char *entrada_parseada[],char *caracteres){ // se ingresa cada palabra a una posicion del vector entrada_parseada
	int i=0;
	char *token; 
	token = strtok( entrada, caracteres); //se quitan los "caracteres"
	entrada_parseada[i]=token;// 
	while(token!=NULL){	
		i++;
		token = strtok( NULL, caracteres);//se siguen quitando los "caracteres"
		entrada_parseada[i]=token;
	}
	return i;
}
