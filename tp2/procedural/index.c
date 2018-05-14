/**
* @mainpage Comunicación de procesos en computación paralela
*
* ===================
*
* @author Sepulveda Federico (federico.sepulveda@alumnos.unc.edu.ar)
* @date Mayo, 2018
*
* @brief Esta aplicacion en un procesador dopple, que lee los datos que son muestra de un ADC de un archivo binario pulso.iq
* y devuelve un resultado en un archivo binaro que contiene la autocorrelacion de los pulsos recibidos.
* Lenguaje: C.
* 
* @par INTRODUCCIÓN:
* Esta aplicacion esta desarrollada de dos maneras, una sin explotar el paralelismo y la otra explotando el paralelismo utilizando
* la biblioteca OpenMPI
*
* @par INSTALACIÓN y UTILIZACIÓN:
* Para la aplicaion procedural solo basta con ejecutar la aplicacion y esta genera un archivo binario llamado resultados con los datos\n
* A continuación se detallan la ejecucion:\n
* 
* 	En el directorio /tp1/tp1_servidor_baash ejecutar:
* 	"make"									--> Compila el proyecto y genera ejecutable.
* 	"./tp2_procedural"				--> Corre la aplicacion.
*
* Para la aplicaion paralela se debe ejecutar pasando como parametro la cantidad de hilos\n
* y esta genera un archivo binario llamado resultados con los datos\n
*
* 	En el directorio /tp1/tp1_cliente_baash ejecutar:
* 	"make"									--> Compila el proyecto y genera ejecutable.
* 	"./tp2_procedural <Numero de Hilos>"					--> Corre la aplicacion con la cantidad de hilos pasada como parametro.
*
* 
*/
