/**
* \file sock_cli_i_cc.c
* \brief Aplicación de Cliente baash. 
* \author Sepulveda Federico
* */
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/// Tamaño de los buffer para sockets 
#define TAM 2048

/*!
  Función principal del cliente.
*/
int parsear_entrada(char *entrada,char *entrada_parseada[],char *caracteres);
int autenticar(int sockfd,char *usuario);

int main( int argc, char *argv[] ) {
	int sockfd, puerto,puerto_udp,sockfdUDP, n,fin=0;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	int longitud=0,autenticacion=0;
	char* entrada_parseada[TAM];
	char entrada[TAM];
	char buffer[TAM];
	char* buffer_parseado[TAM];
	char buffer_axiliar[TAM];
	char buffer_archivo[TAM+1];
	char nombre_archivo[TAM+1];


	printf( "\nPara establecer conexion con el servidor debe ingresar: connect usuario@numero_ip:port\n" );
	printf( "\nCliente: " );
	memset( entrada, '\0', TAM );// se coloca un \0 en entrada en la pocision al final de entrada
	///\par Se le pide al usuario que ingrese connect seguido por usuario@numero_ip:puerto 
	fgets( entrada, TAM-1, stdin );//se lee el archivo stdin (lee lo que se ingreso) y se copia en entrada DESCOMENTAR

	// strcpy(entrada,"connect sepulveda@192.168.0.6:6020");//SOLO PARA PRUEBA DESCOMENTAR ARRIBA Y ABAJO
	entrada[strlen(entrada)-1] = '\0';// se elimina el \n que es introducido por el usuario cuando preciona "enter" DESCOMENTAR
	longitud=parsear_entrada(entrada,entrada_parseada," @:");

	while( strcmp( "connect", entrada_parseada[0]) ){
			printf( "\nComando desconocido debe ingresar connect seguido usuario@numero_ip:puerto" );
			printf( "\nCliente: " );
			memset( entrada, '\0', TAM );// se coloca un \0 en entrada en la pocision al final de entrada
			fgets( entrada, TAM-1, stdin );//se lee el archivo stdin (lee lo que se ingreso) y se copia en entrada 
			entrada[strlen(entrada)-1] = '\0';// se elimina el \n que es introducido por el usuario cuando preciona "enter"
			longitud=parsear_entrada(entrada,entrada_parseada," @:");
			
	}

	if ( longitud < 2 ) {
		fprintf( stderr, "Direccion IPv4 inválida.\n");
		exit( 0 );
	}

	puerto = atoi(entrada_parseada[3]); //numero de puerto
	sockfd = socket( AF_INET, SOCK_STREAM, 0 );
	if ( sockfd < 0 ) {
		perror( "ERROR apertura de socket" );
		exit( 1 );
	}

	server = gethostbyname( entrada_parseada[2] ); // numero de IP 
	if (server == NULL) {
		fprintf( stderr,"Error, no existe el host\n" );
		exit( 0 );
	}
	
	memset( (char *) &serv_addr, '0', sizeof(serv_addr) );
	serv_addr.sin_family = AF_INET;
	bcopy( (char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length );
	serv_addr.sin_port = htons( puerto );
	if ( connect( sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr ) ) < 0 ) {
		perror( "conexion" );
		exit( 1 );
	}
	///\par Se le indica al usuario cliente que la conexión esta establecida
	printf( "USUARIO %s. Conectado a server de direccion: %s  en puerto: 6020.\n",entrada_parseada[1], entrada_parseada[2]); 
	while(1) {

		///\par Se llama a la funcion autenticacion.
		while(autenticacion==0){
			autenticacion=autenticar(sockfd,entrada_parseada[1]);
		}
		///\par Si fallo la acutenticion se cierra la aplicacion cliente
		if (autenticacion==-1){
			printf( "Supero el numero de intentos pemitidos\n" );
			close(sockfd);
			exit(0);
		}

		printf("%s",entrada_parseada[1]);//nombre de usuario
		memset( buffer, '\0', TAM ); 
		n = read( sockfd, buffer, TAM ); // Recibo
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit( 1 );
		}
		buffer[strlen(buffer)-1] = '\0';
		printf( "%s", buffer );/// se imprime promt()

		fflush(0);
		memset( buffer, '\0', TAM );
		fgets( buffer, sizeof(buffer), stdin );
		// printf("tamaño %d\n", strlen(buffer));
		if (strlen(buffer)==1){ //se detecta que se esta por enviar un caracter nulo
			memset(buffer, '\0', TAM );
			strcpy(buffer,"-1\n");

		}
		
		n = write( sockfd, buffer, strlen(buffer) ); // Envio
		if ( n < 0 ) {
			perror( "escritura de socket" );
			exit( 1 );
		}
		strcpy(buffer_axiliar,buffer);
		longitud=parsear_entrada(buffer_axiliar,buffer_parseado," ");// se parsea la entrada para buscar las variables

		
		memset( buffer, '\0', TAM ); 
		n = read( sockfd, buffer, TAM ); // Recibo
		if ( n < 0 ) {
			perror( "lectura de socket" );
			exit( 1 );
		}
		buffer[strlen(buffer)] = '\n';
		printf( "%s", buffer ); // Recibo ejecucion de comando
		///\par Comando "descargar".
		/// Se abre un archivo con el nombre del archivo que se solicito descargar del servidor 
		///donde se iran almacenando los dato recibidos.
		if((strcmp( "descarga", buffer_parseado[0]))==0){ 
			FILE *datos;
			memset( nombre_archivo, '\0', TAM );
			strcpy(nombre_archivo,buffer_parseado[1]);
			nombre_archivo[strlen(nombre_archivo)-1]='\0'; // se quita el "enter" (\n) del nombre
			datos = fopen(nombre_archivo, "w"); // se crea en el cliente un archivo con el nombre del archivo a descargar
			if (datos == NULL) {	perror("File not found!\n");}
			memset( buffer_archivo, '\0', TAM+1 );
			fin=0;
			puerto_udp=6020;
			//*********************************************Recepcion UDP**********************************************************
			///Se copia el contenido del archivo recibido en un archivo 
			struct sockaddr_in dest_addrUDP;
			sockfdUDP = socket( AF_INET, SOCK_DGRAM, 0 );
			if (sockfdUDP < 0)
			{
				perror( "apertura de socket" );
				exit( 1 );
			}
			dest_addrUDP.sin_family = AF_INET;
			dest_addrUDP.sin_port = htons(puerto_udp);
			dest_addrUDP.sin_addr = *( (struct in_addr *)server->h_addr );
			memset( &(dest_addrUDP.sin_zero), '\0', 8 );
			socklen_t tam_dir = sizeof(dest_addrUDP);
			///\brief 		
			n = sendto( sockfdUDP, (void *)"solicitud", 10, 0, (struct sockaddr *)&dest_addrUDP, tam_dir);
			if ( n < 0 )
			{
				perror( "Escritura en socket" );
				exit( 1 );
			}
			//Esperando respuesta
			///Recibe respuesta del Server de que el archivo se transfirió correctamente
			printf( "CREANDO ARCHIVO %s\n",buffer_parseado[1]);
			while(!fin){
				n = recvfrom( sockfdUDP, (void *)buffer_archivo, TAM, 0, (struct sockaddr *)&dest_addrUDP, &tam_dir);
				if ( n < 0 ){perror( "Lectura de socket" );exit( 1 );}
				// printf( "Respuesta: %s\n", buffer_archivo );
				if( !strcmp( "finDeLectura", buffer_archivo) ){
					fin=1;
				}else{
					n = fwrite(buffer_archivo, sizeof(char), strlen(buffer_archivo), datos);
					if(n < 0){	printf("File not written!\n");}
				}
			}	
			
			//~ printf( "Respuesta: %s\n", buffer_archivo );
			
			//******************************************************
			printf( "ARCHIVO %s CREADO\n",buffer_parseado[1]);
			fclose(datos);

		}
		
	}
	return 0;
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
  \param sockfd es el file descriptor utilizado para realizar la comunicaciones TCP.
  \param usuario es el nombre de usuario.
   \return Devuelve 1 si la autenticacion fue exitosa, -1 si se supero en numero de intentos por parte del servidor y 0 si la clave o usuario son incorrectos.
*/
///\par Función autenticar.
/// Esta funcion se encarga de la autencicacion con el servidor. Se le envia a destino el usuario con la clave ingresada
///con formato "usuario,clave", esta cadena se compara con los datos almacenados en la tabla. 
int autenticar(int sockfd,char *usuario){
	char entrada[TAM],buffer_receptor[TAM],concatenar[TAM];
	int n;
	printf( "Usuario: %s\nIngrese la clave: ", usuario);
	strcpy(concatenar,usuario);
	/// Se le pide al usuario que ingrese la clave correspondiete
	fgets( entrada, TAM, stdin );//se lee el archivo stdin (lee lo que se ingreso) y se copia en entrada 

	strcat(concatenar,",");
	/// Se concatena la el nombre de usuario con la clave y se le da el formato para ser leido por el destino.
	strcat(concatenar,entrada);
	concatenar[strlen(concatenar)-1] = '\0';//quita el \n que esta al final de linea
	/// Se envia la cadena
	n = write( sockfd, concatenar, strlen(concatenar) );//envia usuario/clave
	if ( n < 0 ) {
		perror( "escritura de socket" );
		exit( 1 );
	}
	/// Se espera respuesta por parte del servidor
	n = read( sockfd, buffer_receptor, TAM );//leer socket (respueta OBLIGATORIA por parte del servidor)
	if ( n < 0 ) {
		perror( "lectura de socket" );
		exit( 1 );
	}
	/// - Si es "ok" significa que la clave ingresada fue correcta.
	if( !strcmp( "ok", buffer_receptor) ){
		printf("\nIngreso con exito\n");
		return 1; 
	}
	/// - Si es "superoIntentos" significa que finalizo la autenticacion por superar la cantidad de intentos
	else if( !strcmp( "superoIntentos", buffer_receptor) ) {
		printf("\nSupero la cantidad de intentos\n");
		return -1; 
	}
	/// - Si es "no" es porque la clave o usuarios no son correctos
	else{
		printf("\nNombre de usuario y/o contraseña incorrecto\n");
		return 0;	
	}

}