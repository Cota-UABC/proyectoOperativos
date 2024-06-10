#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

void uso(void);
void crear(char*, char*, int);

int main(int argc, char *argv[])
{
    if(argc != 5) 
    {
        printf("Parametros incorrectos\n");
        uso();
        return 1;
    }

    //parametros
    else if (strcmp(argv[1], "-c") == 0)
    {
        crear(argv[2], argv[3], atoi(argv[4]));
    }
    else
    {
        printf("OPCION INVALIDA\n");
        uso();
    }

    return 0;
}

void uso(void)
{
    printf("Uso: crear_archivos -c [RUTA DIRECTORIO] [NOMBRE] [CANTIDAD]\n");
}

void crear(char* dir_location, char* name, int cant)
{
    char filename[256];
    char filepath[512]; 
    

    for (int i = 1; i <= cant; i++) {
        snprintf(filename, sizeof(filename), "%s#%d", name, i);
        
        snprintf(filepath, sizeof(filepath), "%s/%s", dir_location, filename);
        
        int descriptor = open(filepath, O_CREAT | O_WRONLY, 0644);
        if (descriptor == -1) {
            perror("Error al crear el archivo");
            exit(EXIT_FAILURE);
        }
        
        close(descriptor);
    }
    printf("Archivos creados en: %s\n", dir_location);
}