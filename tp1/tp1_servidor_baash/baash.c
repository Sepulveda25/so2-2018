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
#include <sys/socket.h>
#include <netinet/in.h>
#define BUFFSIZE 1024 
#define READ 0
#define WRITE 1

void obtener_entrada(char *entrada,char *argumentos[],char *fichero);
int operadores(char *argumentos[]);
void buscar_path_ejecutar(char *camino,char *argumentos[]);


int baash(int newsockfd,int clilen) { 
	char entrada[BUFFSIZE+1];
	char fichero[BUFFSIZE+1]; 
	char* argumentos [BUFFSIZE+1];// = { "ls", "-l", "/usr/include", 0 };
	char* argumentosEntrada [BUFFSIZE+1];
    char* argumentosSalida [BUFFSIZE+1];
    char posicion[BUFFSIZE+1];
    char hostname[BUFFSIZE+1];
    char buffer[BUFFSIZE+1];
    struct cli_addr;
	pid_t child_pid;
	int status;
	int fd;
	int pipe_fds[2];
	int n=0;
	//Variable para socket UDP
	int sockfdUDP;
	int puerto_udp = 6020;
	//Variables de configuracion archivos
	FILE *para_enviar;
	char buffer_archivo[BUFFSIZE]; 	
	
	//***********************Cofiguracion Socket UDP*************************
	struct sockaddr_in serv_addrUDP;
	sockfdUDP = socket( AF_INET, SOCK_DGRAM, 0 );
	if (sockfdUDP < 0) { 
		perror("ERROR en apertura de socket");
		exit( 1 );
	}

	memset( &serv_addrUDP, 0, sizeof(serv_addrUDP) );
	serv_addrUDP.sin_family = AF_INET;
	serv_addrUDP.sin_addr.s_addr = INADDR_ANY;
	serv_addrUDP.sin_port = htons( puerto_udp);
	memset( &(serv_addrUDP.sin_zero), '\0', 8 );

	if( bind( sockfdUDP, (struct sockaddr *) &serv_addrUDP, sizeof(serv_addrUDP) ) < 0 ) {
		perror( "ERROR en binding" );
		exit( 1 );
	}
	//********************************************************************************
	dup2(newsockfd, 1);// se desvia el STDOUT
	dup2(newsockfd, 0);// se desvia el STDIN
	while(1){
		int operacion=0;
		int comando;
		int i=0;
		// memset(entrada, '\0', BUFFSIZE );
		// printf("strlen(entrada)=%d\n", strlen(entrada));
		memset(entrada, '\0', BUFFSIZE );
		strcpy(entrada," ");
		
		while ((strlen(entrada)==1)){//// control ingreso de enter solo
			memset(posicion, '\0', BUFFSIZE );
			//direccion actual de pwd
			getcwd(posicion,sizeof(posicion));
			//Se obtiene el nombre de Host
			gethostname(hostname, BUFFSIZE+1);
			printf("@%s:%s$  ",hostname, posicion);
			
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
				}else{
					printf(" \n"); 
				}
			}
			///\par Comando "descargar".
			/// Esta parte lee el archivo que se solicito.
			///Se recorre por lineas y se comienza enviar los datos al 
			///cliente utilizando el socket UDP. Cuando se llega al final del archivo se envia la 
			///cadena "finDeLectura" al cliente, que este interpreta que no hay mas datos para recibir.
			///Una vez finalizado el envio el achivo se cierra.
			else if(strcmp(argumentos[0],"descarga")==0){

				printf("Preparando archivo %s....\n",argumentos[1]); // respueta OBLIGATORIA
				para_enviar = fopen(argumentos[1],"r"); 
				
				//******************** envio UDP********************************
				
				/// Se realiza la configuracion para envio por protocolo UDP
				n = recvfrom( sockfdUDP, buffer, BUFFSIZE-1, 0, (struct sockaddr *)&serv_addrUDP, &clilen );
				if ( n < 0 ) {
					perror( "UDP lectura de socket" );
					exit( 1 );
				}
				// printf( "Recibí_UDP: %s\n", buffer );
				/// Se comienza a leer el archivo y enviar los datos leidos
				fgets(buffer_archivo, BUFFSIZE, para_enviar);
				while (!feof (para_enviar)){
					n = sendto( sockfdUDP, (void *)buffer_archivo, BUFFSIZE, 0, (struct sockaddr *)&serv_addrUDP, clilen  );
					if ( n < 0 ) {perror( "escritura en socket" );exit( 1 );}
					fgets(buffer_archivo, BUFFSIZE, para_enviar);		

				}
				/// Se avisa al destino que no hay mas datos por enviar
				n = sendto( sockfdUDP, (void *)"finDeLectura", 13, 0, (struct sockaddr *)&serv_addrUDP, clilen);		
			
				//**************************************************************
			
				fclose(para_enviar); //se cierra el archivo
			}
			else{
				if(operacion==2){ // redireccionar la entrada
					// printf("*** Redireccionar la entrada ***");
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
					// printf("\n*** Pipe ***");
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
					// printf("Pase por ejecucion normal \n");
					if(strlen(entrada)!=1){
						buscar_path_ejecutar(fichero,argumentos);
					}
					else{
						exit(0);
					}
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
    char prefix[BUFFSIZE+1] = "-1";
    // memset(prefix, '\0', BUFFSIZE);
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
		// printf("\nprefix es :%d", strlen(prefix));
		if(strcmp(prefix,"-1")==0)
		{
			printf("No existe el comando\n");
			// printf("este es verdadero\n" );
			exit(0);
		}
		else
		{
			// printf("este es falso\n");
			execv (prefix, argumentos);
			// exit(0);
		}	             
        	
	}	
	return;
}

