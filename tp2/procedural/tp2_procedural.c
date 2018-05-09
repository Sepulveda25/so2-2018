#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define RANGO_MAX 250
#define RESOLUCION 0.5
#define PULSOS_TOTAL 800
// #define TOTAL_GATES RANGO_MAX/RESOLUCION

int main(int argc, char const *argv[])
{	
	FILE *pulsos;// puntero al achivo pulsos.iq
	unsigned short valid_samples=0; // cantidad de muestras por canal
	int i;
	int prueba=0;
	float samples=0;
	float pulso=0;
	float gate_pulso [500][PULSOS_TOTAL];
	unsigned short resto=0;
	// float suma_pulso_i=0,suma_pulso_q=0;	
	pulsos = fopen("../pulsos.iq","rb");

	

	while (!feof (pulsos)){

		fread( &valid_samples, sizeof(unsigned short), 1, pulsos);// se obtiene el valor validSamples
		unsigned short muestra_gate = RESOLUCION *valid_samples/RANGO_MAX; //muestra por gate
		int gates_total=RANGO_MAX/RESOLUCION;
		resto=valid_samples%gates_total; // resto de muestras
		int intervalo=valid_samples/resto; // intervalo en el que se corrige el numero no entero de muetras por gate
		float suma_pulso_i=0;
		float suma_pulso_q=0;
		int n=0,correccion=0;
		for(i=0;i<(valid_samples*2);i++) // ciclo para V_i,V_q y H_i H_q de cada pulso
		{
			fread( &pulso, sizeof(float), 1, pulsos); //pulsos i
			suma_pulso_i=suma_pulso_i+pulso;

			fread( &pulso, sizeof(float), 1, pulsos); //pulsos q
			suma_pulso_q=suma_pulso_q+pulso;
			n++;
			if((!(i%intervalo))&&(resto>0)) // se corrige cada la cantidad de muestras cada cierto intervalo
			{
				resto--;
				n--; //se le agrega una muestra mas para compensar la division no exacta
			}

			if (muestra_gate==n) // se calcula el 
			{
				n=0;
			}
			
		}
		
		prueba++;
	}
	 
	printf("%d\n",valid_samples );
	printf("%f\n",pulso );
	printf("%d\n",resto );
	printf("%d\n", prueba);
	
	fclose(pulsos);
	return 0;
}