#!/bin/bash

# Modo de uso:


if [ $1 ]; then
 	hilos=$1
else

	echo "Se espera parametro cantidad_hilos."
	echo "Ejemplo: ./tp2_paralelo_ejecucion.sh <cantidad_hilos>"
 	exit 1
fi

#Crea archivos de texto a utilizar.
> lista_tiempos_hilos_$hilos.txt

echo "Compilando.."
make clean
make

echo "Ejecucion y medicion de tiempo para $hilos hilos.."
for i in {1..30}
do
   ./tp2_paralelo $hilos
   gprof -b tp2_paralelo gmon.out > profiling_paralelo.txt
   cat profiling_paralelo.txt | grep main | cut -d " " -f7 >> lista_tiempos_hilos_$hilos.txt
done

echo "Resultados en lista_tiempos_hilos_$hilos.txt"