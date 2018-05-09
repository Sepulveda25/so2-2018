#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <complex.h>

#define RANGO_MAX 250
#define RESOLUCION 0.5
#define PULSOS_TOTAL 3200
// #define TOTAL_GATES RANGO_MAX/RESOLUCION

int main(int argc, char const *argv[])
{	
	unsigned short valid_samples=0; // cantidad de muestras por canal
	int i;
	int prueba=0,canal=0;
	float samples=0;
	float pulso=0;
	float gate_pulso[500][PULSOS_TOTAL];
	unsigned short resto=0;
	float suma_pulso_i=0,suma_pulso_q=0;
	FILE *pulsos;// puntero al achivo pulsos.iq	
	pulsos = fopen("../pulsos.iq","rb");

	

	while (!feof (pulsos)){

		fread( &valid_samples, sizeof(unsigned short), 1, pulsos);// se obtiene el valor validSamples
		unsigned short muestra_gate = RESOLUCION *valid_samples/RANGO_MAX; //muestra por gate
		int gates_total=RANGO_MAX/RESOLUCION;
		resto=valid_samples%gates_total; // resto de muestras
		int intervalo=valid_samples/resto; // intervalo en el que se corrige el numero no entero de muetras por gate
		float suma_pulso_i=0;
		float suma_pulso_q=0;
		int n=0,correccion=0,num_gate=0;
		int gate = gates_total * muestra_gate;

		for(i=0;i<(valid_samples*2);i++) // ciclo para V_i,V_q y H_i H_q de cada pulso
		{
			if(gate>0){
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
					// printf("%d  , %d , %d\n", num_gate,canal,prueba);
					if(correccion)
					{
						correccion=0;
						gate_pulso [num_gate][canal]=(float)suma_pulso_i/(muestra_gate+1);
						gate_pulso [num_gate][canal+1]=(float)suma_pulso_q/(muestra_gate+1);

					}else
					{
						gate_pulso [num_gate][canal]=(float)suma_pulso_i/muestra_gate;
						gate_pulso [num_gate][canal+1]=(float)suma_pulso_q/muestra_gate;
					}
					num_gate++;
					suma_pulso_i = 0.000000;
					suma_pulso_q = 0.000000;	
				}

				
			}
			else
			{
				fread( &pulso, sizeof(float), 1, pulsos); 	
	    		fread( &pulso, sizeof(float), 1, pulsos);	
				
			}

			if (i == valid_samples){					
		    	canal=canal+2;
				num_gate=0;
				resto= valid_samples%gates_total;
		    	gate =  (gates_total * muestra_gate)+1;
		    	suma_pulso_i = 0.000000;
				suma_pulso_q = 0.000000;
			}		
		}
		
		canal=0;
		prueba++;
	}
	 

	printf("%d\n", prueba);
	
	fclose(pulsos);
	return 0;
}