#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
/// Tamaño de los buffer para sockets 
#define BUFFSIZE 256 
#define READ 0
#define WRITE 1

void obtener_entrada(char *entrada,char *argumentos[],char *fichero);
int operadores(char *argumentos[]);
void buscar_path_ejecutar(char *camino,char *argumentos[]);
int parsear_entrada(char *entrada,char *entrada_parseada[],char *caracteres);
void buscar_en_PATH(char *camino,char *prefix);
void cantidad_de_retrocesos(char *camino,char *prefix);
void armar_path_relativo(char *camino,char *prefix);
void pipe_argumentos(char *argumentosEntrada[],char *argumentosSalida[],char *argumentosListos[]);

///\par baash.
/// Esta funcion es el baash desarrollado en sistemas operativos I.
int main(void) { 
	char entrada[BUFFSIZE+1];
	char fichero[BUFFSIZE+1]; 
	char* argumentos [BUFFSIZE+1];// = { "ls", "-l", "/usr/include", 0 };
	char* argumentosEntrada [BUFFSIZE+1];
    char* argumentosSalida [BUFFSIZE+1];
    char posicion[BUFFSIZE+1];
    char hostname[BUFFSIZE+1];
    char nombre[BUFFSIZE+1];
    int fd;
    struct cli_addr;
	pid_t child_pid;
	int status;
	int pipe_fds[2];
	char *data;
	char *token;
	int k=0,l=0;
	char buffer_path[BUFFSIZE];
	int fd2 = open("salida_pantalla.txt", O_RDWR |O_TRUNC| O_CREAT, 0777);
	if(fd2 < 0){
        printf("Error al abrir salida_pantalla.txt %d \n", errno);
        return 0;
	}

	// printf("%s%c%c\n", "Content-Type:text/html;charset=iso-8859-1",13,10);
	printf( "Content-Type: text/plain\n\n" );
	memset(entrada, '\0', BUFFSIZE );
	strcpy(entrada," ");
	data = getenv("QUERY_STRING"); // se obtiene el dato de el cuadro de texto 
	// printf("1_Se recibe:  %s\n",data );

	if(data == NULL) {
	  	printf("Error! Error in passing data from form to script.");
	  	return 0;
	}
	else{
		token=strtok (data,"="); // se quita "comando="
		// printf("2_Se recibe:  %s\n",token );
		token=strtok( NULL, "=");// se queda lo que hay despue
		// printf("3_Se recibe:  %s\n",token );

		for (k=0; k<strlen(token);k++){ /// Sa parcea la entrada buscando %2F%3C%3E%7C /<>|
			
			if (*(token+k)=='%'){
				if((*(token+k+1)=='2')&&(*(token+k+2)=='F')){ // se detecta "/"
					// printf("hay /\n");
					entrada[l]='/';

					k=k+2;// avanza al proximo caracter
				}
				else if((*(token+k+1)=='3')&&(*(token+k+2)=='C')){ // se detecta "<"
					// printf("hay <\n");
					entrada[l]='<';
					k=k+2;// avanza al proximo caracter
				}
				else if((*(token+k+1)=='3')&&(*(token+k+2)=='E')){ // se detecta ">"
					// printf("hay >\n");
					entrada[l]='>';
					k=k+2;// avanza al proximo caracter

				}else if((*(token+k+1)=='3')&&(*(token+k+2)=='7')){ // se detecta "|"
					// printf("hay |\n");
					entrada[l]='|';
					k=k+2;// avanza al proximo caracter
				}
			}else{
				entrada[l]=(*(token+k));
				// printf("token --->%c\n", (*(token+k)));
				// printf("entrada--->%c\n", entrada[l]);
			}
			l++;
		}
		entrada[l+1]='\0';	

		///Se lee el path guardado anteriormente
		//************************************************************************************
		FILE *path_anterior; 						
		
		memset(buffer_path, '\0', BUFFSIZE );
		path_anterior = fopen("path.txt","r");

		/// Se lee el archivo path.txt 
		fgets(buffer_path, BUFFSIZE, path_anterior);
		// printf("path: %s\n", buffer_path);

		buffer_path[strlen(buffer_path)]='\0';	
		fclose(path_anterior);//se cierra el archivo
		///******************************************************************************
		//strcpy(entrada,token); // se copia la entrada de datos ya parseada
		// printf("Entrada:  %s\n",entrada );
		// printf("Entrada len:  %d\n",strlen(entrada));
		// printf("buffer_path len:  %d\n",strlen(buffer_path) );

		for (k=0; k<strlen(entrada);k++) if (entrada[k]=='+')entrada[k]=' '; // Se reemplazan los "+" por espacios	
		memset(posicion, '\0', BUFFSIZE );
		//direccion actual de pwd
		getcwd(posicion,sizeof(posicion));
		//Se obtiene el nombre de Host
		gethostname(hostname, BUFFSIZE+1);
		//Se obtiene el nombre de usuario
		getlogin_r(nombre,sizeof(nombre));
		
		if(strlen(buffer_path)==0){
			printf("%s@%s:%s$ %s \n",nombre,hostname, posicion,entrada);
		}else{
			printf("%s@%s:%s$ %s \n",nombre,hostname, buffer_path,entrada);
		}
		

		if (((strlen(entrada)==1))||(entrada[0]==32)){ /// Deteccion de enter solo
				// printf("\n"); /// Respuesta obligatoria al cliente 
			printf("No existe comando \n");
			return 0;
		}
	}
	
	
	int operacion=0;
	int i=0;
	i=parsear_entrada(entrada,argumentos," +");
	strcpy(fichero,entrada);// fichero

	operacion=operadores(argumentos);
	
	int path_nuevo = open("path.txt", O_RDWR |O_TRUNC, 0777);
	if(path_nuevo < 0){
        printf("Error al abrir path.txt cod. %d \n", errno);
        // return 0;
	}

	child_pid = vfork(); 
	if ( child_pid < 0 ) {
		perror( "fork" );
		exit( 1 );
	}

	if ( child_pid == 0 ) {  // Proceso hijo
		/// Se desvia el STDOUT al archivo salida_pantalla.txt
		dup2(fd2, WRITE);
		if(strlen(buffer_path)!=0){ // se cambia el path si existe
			if(chdir(buffer_path)==-1){
				// printf("No existe el fichero o directorio \n");
			}

		}
		
		if(strcmp(argumentos[0],"cd")==0){ // se fija si es el comando cd
			if(chdir(argumentos[1])==-1){
				printf("No existe el fichero o directorio \n");
			}else{
				//************************

				memset(buffer_path, '\0', BUFFSIZE );
				strcpy(buffer_path,argumentos[1]); // se reemplaza el path por el nuevo
				// write(path_nuevo, argumentos[1],strlen(argumentos[1]));

				// printf("path: %s\n", argumentos[1]);
			
				//************************
			}

				char *const parmList[] = {"/bin/echo", NULL};
				execv ("/bin/echo", parmList);
		
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
				if(strlen(entrada)!=1){
					buscar_path_ejecutar(fichero,argumentos);
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
	write(path_nuevo, buffer_path,strlen(buffer_path)); /// se guarda el path actual
	close(path_nuevo);//se cierra el archivo
	close(fd2);//se cierra el archivo
	FILE *salida_baash; 						
	char buffer_archivo[BUFFSIZE];
	salida_baash = fopen("salida_pantalla.txt","r");

	/// Se comienza a leer el archivo salida_pantalla.txt e imprimir el contenido
	fgets(buffer_archivo, BUFFSIZE, salida_baash);
	// printf("%s\n", buffer_archivo);
	while (!feof (salida_baash)){
		printf("%s\n", buffer_archivo);
		fgets(buffer_archivo, BUFFSIZE, salida_baash);		

	} 
	fclose(salida_baash);//se cierra el archivo
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
	else {
		// printf("\nPATH ver en $PATH");
		buscar_en_PATH(camino,prefix);                     // comando solo como date. hay que buscar ruta en $PATH
		// printf("\nprefix es :%d", strlen(prefix));
		if(strcmp(prefix,"-1")==0)
		{
			char *const parmList[] = {"/bin/echo", "No existe el comando...", NULL};
			execv ("/bin/echo", parmList);
		}
		else
		{
			execv (prefix, argumentos);
		}	             
	}	
	return;
}

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

/*!
  \param camino es lo que queremos buscar por ejemplo date, ls, man.
  \param prefix es donde se concatena para armar el path absoluto.
*/
///\par Función buscar_en_PATH.
//concatenamos con cada uno de las rutas de path separadas por : y probamos si existe el acceso. si existe, devolvemos la ruta que funciona con aux
void buscar_en_PATH(char *camino,char *prefix){
    int i=0;
    char contenido_path[BUFFSIZE+1];
    char *contenido_path_parseado[BUFFSIZE+1];
    char path_temporal[BUFFSIZE+1];

    memset(contenido_path, '\0', sizeof(contenido_path));
    memset(path_temporal, '\0', sizeof(path_temporal));
    strcpy(contenido_path,getenv("PATH"));
    // printf("\n%s",getenv("PATH") );

    //se parsea el contenido de path ingresado
	parsear_entrada(contenido_path,contenido_path_parseado,":");

	while(contenido_path_parseado[i]!=NULL){
		// printf("\n%s",contenido_path_parseado[i] );
		strcat(path_temporal,contenido_path_parseado[i]);
		strcat(path_temporal,"/");
		strcat(path_temporal,camino);
		if(access(path_temporal,F_OK)!=-1){// Si no existe la ruta completa (camino + comando, no hace nada y vuelve a entrar con el camino que sigue.
			// printf("\n Existe camino en %s", path_temporal);
			strcpy(prefix,path_temporal);

		}  
		
		// printf("\n%s",path_temporal );
		memset(path_temporal, '\0', sizeof(path_temporal));
		i++;
	}
		
    return ;
}

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

///\par Función armar_path_relativo.
/// Si el path empieza con "./" esta funcion se encarga de pasarlo a un PATH absoluto
void armar_path_relativo(char *camino,char *prefix){
	int i1=0,i=0;
    char * camino_parseado[BUFFSIZE+1];
    i1=parsear_entrada(camino,camino_parseado,"./");
    char posicion[BUFFSIZE+1];
    getcwd(posicion,sizeof(posicion));//direccion actual de pwd
	strcat( prefix, posicion );
    for (i = 0; i < i1; ++i){
        strcat( prefix, "/" );		//se concatena las barras a prefix 
        strcat( prefix, camino_parseado[i] ); //luego se completa para tener   
    }								//el PATH absoluto
	return;
}

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
