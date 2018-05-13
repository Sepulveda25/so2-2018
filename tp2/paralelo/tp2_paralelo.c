#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include <omp.h>

#define RANGO_MAX 250
#define RESOLUCION 0.5
#define PULSOS_TOTAL 1600

int main(int argc, char const *argv[])
{
	unsigned short valid_samples=0; // cantidad de muestras por canal
	int i,j;
	int canal=0;
	float pulso=0;

	float gate_pulso[500][PULSOS_TOTAL];
	

	// unsigned short resto=0;
	FILE *pulsos;// puntero al achivo pulsos.iq	
	pulsos = fopen("../pulsos.iq","rb");
	
	int cant_pulsos=0;
	float modulo[2],producto_acumulado=0;
	int gates_total=RANGO_MAX/RESOLUCION;

	FILE *resultados;
	resultados=fopen("resultados","wb");

	
	/// Se controla que la ejecucion tenga como parametro la cantidad de hilos
	if ( argc <2 ) {
        printf("Se espera parametro cantidad de hilos \n");
		exit( 1 );
	}
	/// Se obtiene la cantidad de hilos como parametro
	int num_threads= atoi(argv[1]);
	
	omp_set_num_threads(num_threads);

#pragma omp critical (read_from_file)
{
	while (!feof (pulsos)){

		fread( &valid_samples, sizeof(unsigned short), 1, pulsos);// se obtiene el valor validSamples
		unsigned short muestra_gate = RESOLUCION *valid_samples/RANGO_MAX; //muestra por gate
		
		unsigned short resto=valid_samples%gates_total; // resto de muestras
		int intervalo=valid_samples/resto; // intervalo en el que se corrige el numero no entero de muetras por gate
		float suma_pulso_i=0;
		float suma_pulso_q=0;
		int n=0,correccion=0,num_gate=0;
		float real_aux=0;
		float compl_aux=0;

		for(i=0;i<(valid_samples*2);i++) // ciclo para V_i,V_q y H_i H_q de cada pulso
		{

			fread( &pulso, sizeof(float), 1, pulsos); //pulsos i
			suma_pulso_i=suma_pulso_i+pulso;

			fread( &pulso, sizeof(float), 1, pulsos); //pulsos q
			suma_pulso_q=suma_pulso_q+pulso;
			n++;
			if((!(i%intervalo))&&(resto>0)) // se corrige la cantidad de muestras cada cierto intervalo
			{
				resto--;
				n--; //se le agrega una muestra mas para compensar la division no exacta
				correccion=1;
				
			}

			if (muestra_gate==n) // se calcula el promedio de la muestra
			{
				n=0;
				if(correccion==1) // se guarada un la matriz gate_pulso el promedio de las muestras con la correccion
				{
					correccion=0;
					real_aux = (suma_pulso_i/(float)(muestra_gate+1));
				    compl_aux =(suma_pulso_q/(float)(muestra_gate+1));

				}else
				{
					real_aux = (suma_pulso_i/(float)(muestra_gate));
				    compl_aux =(suma_pulso_q/(float)(muestra_gate));
				}
				gate_pulso [num_gate][canal]=real_aux+I*compl_aux;

				num_gate++;
				suma_pulso_i = 0;
				suma_pulso_q = 0;	
			}

			if (i == (valid_samples-1)){					
		    	canal++;
				num_gate=0;
				resto= valid_samples%gates_total;
		    	suma_pulso_i = 0;
				suma_pulso_q = 0;
				
			}		
		}
		
		canal=0;
		cant_pulsos++;
	}

	// printf("%d\n",cant_pulsos );
}

	
	cant_pulsos--; 


#pragma omp sections 
{
	/// Se calcula la autocorrelacion para la matriz de datos gate_pulso y se va guardando el resultado un achivo binario
    for(i=0;i<gates_total;i++)
	{
		// private(j) hace que cada hilo tenga una copia privada de la variable j.
		// reduction(+:producto_acumulado): La cláusula de reducción es una mezcla entre las cláusulas privadas, compartidas y atómicas.
		// Permite acumular una variable compartida sin la cláusula atómica.
		// Con schedule(static) al ingresar al ciclo cada hilo decide independientemente qué parte del ciclo procesará.
		#pragma omp parallel for private(j) reduction(+:producto_acumulado) schedule(static)
		for(j=0;j<(cant_pulsos-1);j++){
			/// Primero se calcula en modulo para cada señal i y q
			modulo[0]=cabsf(gate_pulso[i][j]); //Vi 
			modulo[1]=cabsf(gate_pulso[i][j+1]); //Vi+1

			/// Se suman los productos
			producto_acumulado=producto_acumulado+(modulo[0]*modulo[1]); // Vi*Vi+1

		}
		/// Se calcula el promedio de los productos acumulados
		producto_acumulado=producto_acumulado/cant_pulsos;
		/// Se guarda en el archivo resultados los valores de la autocorrelacion de c/gate 
		#pragma omp critical (write_to_file) /// Seccion critica al escribir el archivo con los resultados
 		fwrite(&producto_acumulado, sizeof(float), 1, resultados);
		producto_acumulado=0;

	}
}
	
	fclose(pulsos);
	fclose(resultados);
	return 0;
}