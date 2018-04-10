/**
* \file sock_srv_i_cc.c
* \brief Aplicación de Server baash. 
* \author Sepulveda Federico
* */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "baash.h"
#define TAM 256
#define cantida_intentos 4

int main( int argc, char *argv[] ) {
	int sockfd, newsockfd, puerto, clilen, pid;
	char buffer[TAM];
	struct sockaddr_in serv_addr, cli_addr;
	int n,autenticacion=0;

	if ( argc < 2 ) {
        	fprintf( stderr, "Uso: %s <puerto>\n", argv[0] );
		exit( 1 );
	}

	sockfd = socket( AF_INET, SOCK_STREAM, 0);
	if ( sockfd < 0 ) { 
		perror( " apertura de socket ");
		exit( 1 );
	}

	memset( (char *) &serv_addr, 0, sizeof(serv_addr) );
	puerto = atoi( argv[1] );
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons( puerto );

	if ( bind(sockfd, ( struct sockaddr *) &serv_addr, sizeof( serv_addr ) ) < 0 ) {
		perror( "ligadura" );
		exit( 1 );
	}

        printf( "Proceso: %d - socket disponible: %d\n", getpid(), ntohs(serv_addr.sin_port) );

	listen( sockfd, 5 );
	clilen = sizeof( cli_addr );

	while( 1 ) {
		newsockfd = accept( sockfd, (struct sockaddr *) &cli_addr, &clilen );
		if ( newsockfd < 0 ) {
			perror( "accept" );
			exit( 1 );
		}
		dup2(newsockfd, 1);// se desvia el STDOUT
		dup2(newsockfd, 0);// se desvia el STDIN
		pid = fork(); 
		if ( pid < 0 ) {
			perror( "fork" );
			exit( 1 );
		}

		if ( pid == 0 ) {  // Proceso hijo
			close( sockfd );
			// ///\par Se realiza la autenticacion. Espera la contraseña por parte del usuario. Solo tiene cuatro intentos.
			// autenticacion=autenticar(newsockfd);

			while ( 1 ) {
				
  		// 		if(autenticacion==0){
				// 	printf( "PROCESO %d. Se supero los intentos de autenticacion, termino la ejecución.\n\n", getpid() );
				// 	close( sockfd );close( newsockfd );
				// 	exit(0);
				// }
				// memset( buffer, '\0', TAM );

				// n = read( newsockfd, buffer, TAM-1 );
				// if ( n < 0 ) {
				// 	perror( "lectura de socket" );
				// 	exit(1);
				// }

				//printf( "Server->PROCESO %d. ", getpid() );
				//printf( "Recibí: %s", buffer );
				
				// n = write( newsockfd, "Obtuve su mensaje", 18 );
				// if ( n < 0 ) {
				// 	perror( "escritura en socket" );
				// 	exit( 1 );
				// }
				// Verificación de si hay que terminar
				buffer[strlen(buffer)-1] = '\0';
				if( !strcmp( "fin", buffer ) ) {
					printf( "PROCESO %d. Como recibí 'fin', termino la ejecución.\n\n", getpid() );
					exit(0);
				}
				baash();
			}
		}
		else {
			//printf( "SERVIDOR: Nuevo cliente, que atiende el proceso hijo: %d\n", pid );
			close( newsockfd );
		}
	}
	return 0; 
} 



/*!
  \param newsockfd es el file descriptor utilizado para realizar la comunicaciones TCP.
   \return Devuelve 1 si la clave fue correcta 0 si no lo fue.
*/
///\par Función autenticacion.
/// Se utiliza un archivo usuarios.csv inicialmente pre cargado (si no la aplicacion no va a poder funcionar) con nomebres de usuarios
///y las correspondientes claves. Se espera en mensaje por parte del cliente para realizar las comparaciones con los datos del archivo.
///En total el cliente tiene 4 intentos de ingresar la clave de forma incorrecta en caso que no lo logre en niguno de esos intentos se 
///avisa al cliente y se retorna 0. En caso de una conicidencia exitosa se le avisa al usuario y retorna 1.

int autenticar(int newsockfd){
	int n, logitud;
	/**
	* \par Variable intentos:
	* Numero de oportunidades que tiene el usuario para intentar ingresar mal la clave.
	*/
	int intentos=cantida_intentos;
	char buffer[TAM], usuario[TAM],buffer_archivo[TAM];
	char* buffer_parseado[TAM];

	FILE *usuarios;
	// usuarios = fopen("usuarios.csv","r");
	
	while(intentos>0){
		/// Se lee el archivo usuarios.csv el cual es una tabla usuario/clave
		usuarios = fopen("usuarios.csv","r");
		memset( buffer_archivo, '\0', TAM );
		memset( buffer, '\0', TAM );
		/// Espera usuario/clave por parte del cliente		
		n = read( newsockfd, buffer, TAM ); 
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit(1);
		}
		/// Comienza a comparar el dato recibido con todos los datos en la tabla usuarios.csv
		fgets(buffer_archivo, TAM, usuarios);
		while (!feof (usuarios)){
			
			buffer_archivo[strlen(buffer_archivo)-1] = '\0';
			// buffer[strlen(buffer)-1] = '\0';
			/// Si los datos coinciden se avisa a destino que la secion pudo iniciar con exito y se retorna 1.
			if( !strcmp( buffer_archivo,buffer) ){
				//se encotro el usuario con la clave
				n = write( newsockfd, "ok", TAM ); // avisa al destino que se encontro usuario/clave
				if ( n < 0 ) { 
					perror( "lectura de socket" );
					exit( 1 );
				}
				fclose(usuarios);
				memset( buffer, '\0', TAM );
				return 1;
			}
			memset( buffer_archivo, '\0', TAM );		
			fgets(buffer_archivo, TAM, usuarios);		
		}
		if(intentos>1){
			/// En caso de que no coincidan se avisa a destino que la clave o usuario no son correctos
			n = write( newsockfd, "no", TAM ); // avisa al destino que no se encontro usuario/clave
			if ( n < 0 ) { 
				perror( "lectura de socket" );
				exit( 1 );
			}
			
		}
		/// Se resta 1 a intentos y vuelve a empezar
		intentos--;
		fclose(usuarios);
			
	}
	/// Si intentos llego a 0 quiere decir que se supero la cantidad de intentos, por lo tanto se le avisa al cliente
	n = write( newsockfd, "superoIntentos", TAM);
	if ( n < 0 ) {
		perror( "lectura de socket" );
		exit(1);
	}
	// fclose(usuarios);
	return 0;
}