#include "parsear_entrada.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFFSIZE 256 


/*!
  \param camino es lo que queremos buscar por ejemplo date, ls, man.
  \param prefix es donde se concatena para armar el path absoluto.
*/
///\par Función buscar_en_PATH.
//concatenamos con cada uno de las rutas de path separadas por : y probamos si existe el acceso. si existe, devolvemos la ruta que funciona con aux
void buscar_en_PATH(char *camino,char *prefix){
    int i=0;
    char contenido_path[BUFFSIZE+1];
    char *contenido_path_parseado[BUFFSIZE+1];
    char path_temporal[BUFFSIZE+1];

    memset(contenido_path, '\0', sizeof(contenido_path));
    memset(path_temporal, '\0', sizeof(path_temporal));
    strcpy(contenido_path,getenv("PATH"));
    // printf("\n%s",getenv("PATH") );

    //se parsea el contenido de path ingresado
	parsear_entrada(contenido_path,contenido_path_parseado,":");

	while(contenido_path_parseado[i]!=NULL){
		// printf("\n%s",contenido_path_parseado[i] );
		strcat(path_temporal,contenido_path_parseado[i]);
		strcat(path_temporal,"/");
		strcat(path_temporal,camino);
		if(access(path_temporal,F_OK)!=-1){// Si no existe la ruta completa (camino + comando, no hace nada y vuelve a entrar con el camino que sigue.
			// printf("\n Existe camino en %s", path_temporal);
			strcpy(prefix,path_temporal);

		}  
		
		// printf("\n%s",path_temporal );
		memset(path_temporal, '\0', sizeof(path_temporal));
		i++;
	}
		
    return ;
}

