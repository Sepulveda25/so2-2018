#include <stdio.h>
#include <stdlib.h>
#include <time.h>       /* time_t, time, ctime */

int main(void)
{
    FILE *file1;
	char buffer[100];
    time_t timer;    
    struct tm* tm_info;
	printf( "Content-Type: text/plain\n\n" );

    /// Se imprime info de procesador
    printf("\nProcesador:\n");
    printf("-----------\n");
    file1 = fopen("//proc//cpuinfo", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }   
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
    printf("%s", buffer);
    
    fclose(file1);

    /// Se imprime info de memoria
    printf("\nMemoria:\n");
    printf("--------\n");
    file1 = fopen("//proc//meminfo", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }   
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
    printf("%s", buffer);
    
    fclose(file1);

    /// Se imprime el uptime
    printf("\nUptime:\n");
    printf("-------\n");
    file1 = fopen("//proc//uptime", "r");
    if (file1 == NULL)
    {
        perror("file1");
        exit(1);
    }   
    float uptime, idletime;
    int uptime_i, idletime_i;
    fscanf(file1, "%f %f", &uptime, &idletime);
    uptime_i = (int) uptime;
    idletime_i = (int) idletime;
    printf("Uptime: %dseg = %dd%dh%dm%ds\n", uptime_i,(uptime_i/86400), ((uptime_i%86400)/3600), ((uptime_i%1440)/60), (uptime_i%60));
    printf("Idletime: %dseg = %dd%dh%dm%ds\n", idletime_i,(idletime_i/86400), ((idletime_i%86400)/1440), ((idletime_i%1440)/60), (idletime_i%60));
    fclose(file1);
	
    /// Se imprime la fecha y hora actual
    printf("\nFecha y hora actual:\n");
    printf("--------------------\n");
    time(&timer);
    tm_info = localtime(&timer);
    strftime(buffer, 26, "%d-%m-%Y %H:%M:%S", tm_info);    
    printf("%s", buffer);
		
    return 0;
}