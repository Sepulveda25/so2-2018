#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFSIZE 256 

///\par Función pipe_argumentos.
/// Funcion encargada de parsear los comandos para el pipe

void pipe_argumentos(char *argumentosEntrada[],char *argumentosSalida[],char *argumentosListos[]){
    int i=0, j=0, k=0;  
    char tmp[BUFFSIZE];      
    int aux = 0; 
    //printf("argumentosListos %s\n", argumentosListos[i]);
    for (k = 0; k < BUFFSIZE; k++){
        argumentosSalida[k] = NULL;
        argumentosEntrada[k] = NULL;
    }
    while(argumentosListos[i]!=NULL){
        strcpy(tmp,argumentosListos[i]);
        if ( strcmp(tmp, "|") == 0 ){       
            aux = 1;
        }
        else {
            //antes de |
            if (aux == 0){
                argumentosEntrada[i]= argumentosListos[i];
            }
            //despues de |
            else{           
                argumentosSalida[j]= argumentosListos[i]; 
                j++;        
            }
        }
        i++;
    }            
    return;
}
