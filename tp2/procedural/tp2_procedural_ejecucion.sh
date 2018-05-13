#!/bin/bash

# Modo de uso:
# ./tp2_procedural_ejecucion.sh 


#Crea archivos de texto a utilizar.
> lista_tiempos.txt

echo "Compilando.."
make clean
make

echo "Ejecucion y medicion de tiempo.."
for i in {1..30}
do 
   ./tp2_procedural
   gprof -b tp2_procedural gmon.out > ProfilingProcedural.txt
   cat ProfilingProcedural.txt | grep main | cut -d " " -f7 >> lista_tiempos.txt
done

echo "Resultados en lista_tiempos.txt"