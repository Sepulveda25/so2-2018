#include <math.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RANGO_MAX 250
#define RESOLUCION 0.5
#define PULSOS_TOTAL 3200
// #define TOTAL_GATES RANGO_MAX/RESOLUCION

int main(int argc, char const *argv[])
{	
	unsigned short valid_samples=0; // cantidad de muestras por canal
	int i,j;
	int prueba=0,canal=0;
	float samples=0;
	float pulso=0;
	float gate_pulso[500][PULSOS_TOTAL];
	unsigned short resto=0;
	float suma_pulso_i=0,suma_pulso_q=0;
	FILE *pulsos;// puntero al achivo pulsos.iq	
	pulsos = fopen("../pulsos.iq","rb");
	
	int cant_pulsos=0;
	double modulo[2],producto_acumulado=0;
	int gates_total=RANGO_MAX/RESOLUCION;

	//debug
	FILE *salida;
	salida=fopen("salida.CSV","w");
	char buffer[100];


	while (!feof (pulsos)){

		fread( &valid_samples, sizeof(unsigned short), 1, pulsos);// se obtiene el valor validSamples
		unsigned short muestra_gate = RESOLUCION *valid_samples/RANGO_MAX; //muestra por gate
		
		resto=valid_samples%gates_total; // resto de muestras
		int intervalo=valid_samples/resto; // intervalo en el que se corrige el numero no entero de muetras por gate
		float suma_pulso_i=0;
		float suma_pulso_q=0;
		int n=0,correccion=0,num_gate=0;

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
				if(correccion==1)
				{
					correccion=0;
					gate_pulso [num_gate][canal]=(float)suma_pulso_i/(muestra_gate+1);
					gate_pulso [num_gate][canal+1]=(float)suma_pulso_q/(muestra_gate+1);

				}else
				{
					gate_pulso [num_gate][canal]=(float)suma_pulso_i/muestra_gate;
					gate_pulso [num_gate][canal+1]=(float)suma_pulso_q/muestra_gate;
				}
				// memset( buffer, '\0', strlen(buffer));
				// sprintf(buffer, "%f", gate_pulso [num_gate][canal]);
				// strcat(buffer,", ");
				// fwrite(buffer, sizeof(char), strlen(buffer), salida);
				// memset( buffer, '\0', strlen(buffer));
				// sprintf(buffer, "%f", gate_pulso [num_gate][canal+1]);
				// strcat(buffer,", ");
				// fwrite(buffer, sizeof(char), strlen(buffer), salida);


				// printf("%f , %f\n", gate_pulso [num_gate][canal],gate_pulso [num_gate][canal+1]);
				num_gate++;
				suma_pulso_i = 0.000000;
				suma_pulso_q = 0.000000;	
			}

			if (i == (valid_samples-1)){					
		    	canal=canal+2;
				num_gate=0;
				resto= valid_samples%gates_total;
		    	suma_pulso_i = 0.000000;
				suma_pulso_q = 0.000000;
				
				// fwrite("\n", sizeof(char), strlen(buffer), salida);
			}		
		}
		
		canal=0;
		// prueba++;
		// printf("%d , %f\n", prueba,gate_pulso[prueba/2][4]);
		cant_pulsos++;
	}
	
	cant_pulsos--;

	/// Se calcula la autocorrelacion para la matriz de datos gate_pulso y se va guardando el resultado un achivo binario
    for(i=0;i<gates_total;i++)
	{
		for(j=0;j<(cant_pulsos-1);j++){
			/// Primero se calcula en modulo para cada señal i y q
			modulo[0]=sqrt((pow(gate_pulso[i][j*2],2)+pow(gate_pulso[i][(j*2)+1],2))); //Vi 
			modulo[1]=sqrt((pow(gate_pulso[i][(j*2)+2],2)+pow(gate_pulso[i][(j*2)+3],2))); //Vi+1

			// modulo[0]=sqrt((gate_pulso[i][j*2]*gate_pulso[i][j*2])+(gate_pulso[i][(j*2)+1]*gate_pulso[i][(j*2)+1])); //Vi 
			// modulo[1]=sqrt((gate_pulso[i][(j*2)+2]*gate_pulso[i][(j*2)+2])+(gate_pulso[i][(j*2)+3]*gate_pulso[i][(j*2)+3])); //Vi+1

			/// Se suman los productos
			producto_acumulado=producto_acumulado+(modulo[0]*modulo[1]); // Vi*Vi+1

		}
		/// Se calcula el promedio de los productos acumulados
		producto_acumulado=producto_acumulado/cant_pulsos;
		// printf("%f , %f, %f\n", producto_acumulado, modulo[0],modulo[1]);
		producto_acumulado=0.000000;
	}
	
	fclose(pulsos);
	fclose(salida);
	return 0;
}