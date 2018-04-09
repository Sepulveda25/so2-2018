#include <stdio.h>
#include "parsear_entrada.h"
#include "prompt.h"
#include "buscar_en_PATH.h"
#include "cantidad_de_retrocesos.h"
#include "armar_path_relativo.h"
#include "armar_path_home.h"
#include "pipe_argumentos.h"
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#define BUFFSIZE 256 
#define READ 0
#define WRITE 1

void obtener_entrada(char *entrada,char *argumentos[],char *fichero);
int operadores(char *argumentos[]);
void buscar_path_ejecutar(char *camino,char *argumentos[]);


int baash() { 
	char entrada[BUFFSIZE+1];
	char fichero[BUFFSIZE+1]; 
	char* argumentos [BUFFSIZE+1];// = { "ls", "-l", "/usr/include", 0 };
	char* argumentosEntrada [BUFFSIZE+1];
    char* argumentosSalida [BUFFSIZE+1];
	pid_t child_pid;
	int status;
	int fd;
	int pipe_fds[2];

	while(1){
		int operacion=0;
		int comando;
		int i=0;
		
		while (strlen(entrada)==1){ // control ingreso de enter solo
			prompt();
			fflush(0);
			fgets(entrada,sizeof(entrada),stdin);
		}
        entrada[strlen(entrada)-1]='\0';
		
		// obtener_entrada(entrada,argumentos,fichero);

		i=parsear_entrada(entrada,argumentos," ");
		strcpy(fichero,entrada);// fichero

		
		// printf("\nFichero:%s ",fichero); 
		// while(argumentos[j]!=NULL){
		// 	printf("\nArgumentos:%s ",argumentos[j]);
		// 	j++;
		// }
		comando=strcmp(argumentos[0],"exit");
		if ((feof(stdin))||(comando==0))
		{
			printf("\nSaliendo...\n");
			exit(0);
		}
		operacion=operadores(argumentos);
		// printf("\nOperacion:%d ",operacion);
		
		
		child_pid = vfork(); 
		if ( child_pid < 0 ) {
			perror( "fork" );
			exit( 1 );
		}

		if ( child_pid == 0 ) {  // Proceso hijo
			if(strcmp(argumentos[0],"cd")==0){ // se fija si es el comando cd
				if(chdir(argumentos[1])==-1){
					printf("No existe el fichero ó directorio \n");
				}
			}else{
				if(operacion==2){ // redireccionar la entrada
					// printf("\n*** Redireccionar la entrada ***");
					close (0); //0 	Standard Input (stdin) 
					fd = open (argumentos[i-1], O_RDONLY);
					if (fd == -1) {
	  					printf("No se pudo abrir el archivo\n");
	  					exit (1);
					}
					argumentos[i-1]='\0'; // se elimina dir ej /home/
					argumentos[i-2]='\0'; // se elimina <
					buscar_path_ejecutar(fichero,argumentos);
					close (fd);
				} 
				else if(operacion==3){ // redireccionar la salida
					// printf("\n*** Redireccionar la salida ***");
					close (1); //1 	Standard Output (stdout) 
					fd = open (argumentos[i-1], O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
					if (fd == -1) {
	  					printf("No se pudo abrir el archivo\n");
	  					exit (1);
					}
					argumentos[i-1]='\0'; // se elimina dir ej /home/
					argumentos[i-2]='\0'; // se elimina >
					buscar_path_ejecutar(fichero,argumentos);
					close (fd);
				}else if(operacion==4){ // pipe
					printf("\n*** Pipe ***");
					if (pipe (pipe_fds)){
      				 	fprintf (stderr, "Pipe failed.\n");
       					return EXIT_FAILURE;
     				}
     				pipe_argumentos(argumentosEntrada,argumentosSalida,argumentos);
					if(fork()==0) {
	   					close(pipe_fds[READ]);
	   					dup2(pipe_fds[WRITE],1);
						buscar_path_ejecutar(argumentosEntrada[0],argumentosEntrada); //ejecutar lo de la izq de |
	   					close(pipe_fds[WRITE]);
	 				}else{;
	   					close(pipe_fds[WRITE]);
	   					dup2(pipe_fds[READ],0);
						buscar_path_ejecutar(argumentosSalida[0],argumentosSalida); //ejecutar lo de la der de |
	   					close(pipe_fds[WRITE]);
	   					close(pipe_fds[READ]);
	 				}

				}
				else{ // ejecucion normal
					buscar_path_ejecutar(fichero,argumentos);
				}
				

				
			}
		}
		else{//Proceso padre
			if(operacion!=1){
				if (waitpid (child_pid, &status, 0) != child_pid){
		      		status = -1;
				}
			}
		}
	}
	return 0;
} 

/// Se parsea la entrada por teclado que contiene el path o directamente el programa a ejecutar en caso de 
///estar en $PATH (como date) y los parametros para la ejecucion del mismo. El resultado del parseo se guarda en
///la variable argumentos.
void obtener_entrada(char *entrada,char *argumentos[],char *fichero){
	
	parsear_entrada(entrada,argumentos," ");
	strcpy(fichero,entrada);// fichero
	return;
}


/// Parsea argumentos de la linea de comandos. y detecta '&', '<', '>' y '|'
int operadores(char *argumentos[]){
	int operacion=0;
    int i=0;       
    while(argumentos[i]!=NULL){
        if (strcmp(argumentos[i], "&")==0) // operador para proceso en 2do plano
        {
            operacion=1;
        }
        else if (strcmp(argumentos[i], "<")==0) //operador para mandar archivo por stdin a un proceso
        {
            operacion=2;
        }
        else if (strcmp(argumentos[i], ">")==0) // operador para mandar stdout de un proceso hacia un archivo
        {
            operacion=3;
        }
        else if (strcmp(argumentos[i], "|")==0) // operador de pipe (conecta stdout de un proceso con stdin de otro)
        {
            operacion=4;
        }
        i++;
    }            
    return operacion;
}

///Se identifica que tipo de path es (absoluto, relativo, etc)
void buscar_path_ejecutar(char *camino,char *argumentos[]){
    char prefix[BUFFSIZE+1] = "";
    // printf("argumento: %s\n",*(argumentos+1) );

	if(*(camino)=='/'){
		// printf("\nPATH absoluto");
		execv (camino, argumentos);
	}
	else if((*(camino)=='.')&&(*(camino+1)=='/')){ // ./home
		
		// printf("\nPATH relativo (./)");
        armar_path_relativo(camino,prefix);
		// printf("prefix:%s\n", prefix);
		execv (prefix, argumentos);
	}	
	else if((*(camino)=='.')&&(*(camino+1)=='.')&&(*(camino+2)=='/')){	//  ../home
		// printf("\nPATH relativo (../)");
		cantidad_de_retrocesos(camino,prefix);
		// printf("\nprefix:%s\n", prefix);
		execv (prefix, argumentos);
	}
	else if((*(camino)=='~')&&(*(camino+1)=='/')){
		// printf("\nPATH relativo (~/)");
		armar_path_home(camino,prefix);
		// printf("\nprefix:%s\n", prefix);
		 execv (prefix, argumentos);
	}
	else {
		// printf("\nPATH ver en $PATH");
		buscar_en_PATH(camino,prefix);                     // comando solo como date. hay que buscar ruta en $PATH
		// printf("\nprefix:%s\n", prefix);                
        execv (prefix, argumentos);
	}	
	return;
}

