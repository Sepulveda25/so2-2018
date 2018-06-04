#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    FILE *file1;
	char buffer[100];
	printf( "Content-Type: text/plain\n\n" );

    file1 = popen("lsmod", "r");
    if (file1 == NULL)
    {
        perror("modules");
        exit(1);
    }
    while (fgets(buffer, sizeof(buffer), file1) != NULL)
        printf("%s \n", buffer);

    pclose(file1);

    return 0;
}
