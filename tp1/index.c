/**
* @mainpage Conexion SSH
* cliente y servidor
* ===================
*
* @author Sepulveda Federico (federico.sepulveda@alumnos.unc.edu.ar)
* @date Abril, 2018
*
* @brief Este es el resumen de la aplicación. La siguiente aplicación esta conformada por dos aplicaciones
* que conforma la conexion entre un cliente y un servidor que tiene el baash desarrollado en SOI.
* Lenguaje: C.
* 
* @par INTRODUCCIÓN:
* Una de las aplicaciones (el Cliente), se utiliza para conectarse a un Servidor, que es la creado por la otra aplicación.\n
* Se comunican via socket y se pueden ejecutar comandos como si tratara de una terminal de GNU linux. Ademas se le agrego la funcion
* "descarga nombre_archivo" que perminte descargar desde el servirdor el archivo deseado. 
*
* @par INSTALACIÓN:
* (AVISO: tanto la aplicacion cliente como servidor se encuentran en dierectorios distintintos)\n
* A continuación se detallan algunos comandos útiles para el uso de la aplicación:\n
* 
* 	"ip addr"								--> Brinda informacion acerca de las redes del equipo. Es util para averiguar la 
* 												direccion IPv4 del servidor.
* 	
* 	En el directorio /tp1/tp1_servidor_baash ejecutar:
* 	"make"									--> Compila el proyecto y genera ejecutable del server.
* 	"./tp1_servidor_baash 6020"				--> Corre el server y comienza a escuchar por el puerto 6020.
*
* 	En el directorio /tp1/tp1_cliente_baash ejecutar:
* 	"make"									--> Compila el proyecto y genera ejecutable del cliente.
* 	"./tp1_cliente_baash"					--> Corre el cliente.
*
* @par UTILIZACIÓN:
* (AVISO: todos los comandos se ingresan desde el cliente)\n
* A continuacion se listan algunos comandos que reconoce la aplicacion:\n
* 
* 	"descarga nombre_archivo"				--> Descarga en el cliente el archivo solicitado al servidor.
* 	"ls"									--> Muestra los archivos del directorio actual.
* 	"ps"									--> Muestra los procesos que estan corriendo.
* 	"date"									--> Muestra la fecha que tiene el servidor.
*
* 
* @par EJEMPLO:
* (En el Cliente)\n
* A continuación se detalla una secuencia de comandos típica a modo de ejemplo:\n
* 	
* 	"connect usuario@numero_ip:6020"	--> Se conecta al servidor con IP=numero_ip. (averiguar con ip addr)
* 	"pass"								--> Clave correspondiente al usuario 'usuario'. El servidor solo permite 4 (cuatro) intentos 
*											para ingresar la contraseña, una vez superado este numero se cierra la conexion.			
* 	"ls"								--> El servidor respondera con un lista con los nombres de los archivos dentro del directorio actual
* 
*/
