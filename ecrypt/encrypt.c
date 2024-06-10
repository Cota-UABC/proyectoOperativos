#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include<unistd.h>

#define CLAVE_DEFAULT "9c2[v:a$v/!0SCq?w2"
#define BUFFER_LENGHT 20
#define MAX_NAME_LENGHT 256

void uso(void);
void encrypt(char*, char*, char*);
void decrypt(char*, char*, char*);

int main(int argc, char *argv[])
{
    if(argc != 4 && argc != 5) 
    {
        printf("Parametros incorrectos\n");
        uso();
        return 1;
    }

    //parametros
    else if (strcmp(argv[1], "-e") == 0)
    {
        if(argc == 5)
            encrypt(argv[2], argv[3], argv[4]);
        if(argc == 4)
        {
            char *clave_default = CLAVE_DEFAULT;
            encrypt(argv[2], argv[3], clave_default);
        }
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        if(argc == 5)
            decrypt(argv[2], argv[3], argv[4]);
        if(argc == 4)
        {
            char *clave_default = CLAVE_DEFAULT;
            decrypt(argv[2], argv[3], clave_default);
        }
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
    printf("Uso: encrypt [-OPCION] [RUTA ARCHIVO FUENTE] [RUTA ARCHIVO DE SALIDA] [CLAVE]\n\n");
    printf("Opciones:\n");
    printf("    -e      Encriptar\n");
    printf("    -d      Desencriptar\n\n");
    printf("Clave: Opcional\n");
}

void encrypt(char* input_file_location, char* output_file, char* clave) 
{
    int descriptor_origen, descriptor_enc, bytes_lectura, bytes_grabados;
    uint8_t semi_clave=0, buffer[BUFFER_LENGHT];
    uint16_t temp;

    descriptor_origen = open(input_file_location, O_RDONLY);
    if(descriptor_origen == -1) 
    {
        printf("Error al abrir el archivo de origen\n");
        close(descriptor_origen);
        exit(EXIT_FAILURE);
    }

    descriptor_enc = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (descriptor_enc == -1) 
    {
        printf("Error al crear el archivo destino\n");
        close(descriptor_origen);
        close(descriptor_enc);
        exit(EXIT_FAILURE);
    }

    temp = clave[1] | (clave[0]<<8); // tomar dos bytes de la clave
    temp = -temp;// invertir valor

    for(int i=0;i<16;i+=2)// generar semiclave
    {
        if((temp & (1<<i)) > 0)
            semi_clave |= (1<<(i/2));
    }

    uint16_t i=0;
    while ((bytes_lectura = read(descriptor_origen, buffer, 1)) > 0)// encriptar
    {
        buffer[0] = buffer[0] ^ semi_clave;

        buffer[0] -= 0x3e;

        buffer[0] = buffer[0] ^ (uint8_t)clave[i];

        i++;
        if(i == strlen(clave))// repetir la clave en ciclos
            i = 0;
        
        bytes_grabados = write(descriptor_enc,buffer,1);
        if(bytes_grabados == -1)
        {
            printf("Error en la escritura\n");
            close(descriptor_origen);
            close(descriptor_enc);
            exit(EXIT_FAILURE);
        }
    }

    printf("Archivo encriptado correctamente como %s\n",output_file);

    close(descriptor_origen);
    close(descriptor_enc);
}

void decrypt(char* input_file_location, char* output_file, char* clave)
{
    int descriptor_origen, descriptor_enc, bytes_lectura, bytes_grabados;
    uint8_t semi_clave=0, buffer[BUFFER_LENGHT];
    uint16_t temp;

    descriptor_origen = open(input_file_location, O_RDONLY);
    if(descriptor_origen == -1) 
    {
        printf("Error al abrir el archivo de origen\n");
        close(descriptor_origen);
        exit(EXIT_FAILURE);
    }

    descriptor_enc = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (descriptor_enc == -1) 
    {
        printf("Error al crear el archivo destino\n");
        close(descriptor_origen);
        close(descriptor_enc);
        exit(EXIT_FAILURE);
    }

    temp = clave[1] | (clave[0]<<8); // tomar dos bytes de la clave
    temp = -temp;// invertir valor

    for(int i=0;i<16;i+=2)// generar semiclave
    {
        if((temp & (1<<i)) > 0)
            semi_clave |= (1<<(i/2));
    }

    uint16_t i=0;
    while ((bytes_lectura = read(descriptor_origen, buffer, 1)) > 0)// encriptar
    {
        buffer[0] = buffer[0] ^ (uint8_t)clave[i];

        buffer[0] += 0x3e;

        buffer[0] = buffer[0] ^ semi_clave;

        i++;
        if(i == strlen(clave))// repetir la clave en ciclos
            i = 0;
        
        bytes_grabados = write(descriptor_enc,buffer,1);
        if(bytes_grabados == -1)
        {
            printf("Error en la escritura\n");
            close(descriptor_origen);
            close(descriptor_enc);
            exit(EXIT_FAILURE);
        }
    }

    printf("Archivo decriptado como %s\n",output_file);


    close(descriptor_origen);
    close(descriptor_enc);

    descriptor_origen = open(output_file, O_RDONLY);

    uint8_t cont = 0;
    i=0;
    while(cont<5 && i<30)// revisar que el archivo sea leible
    {
        bytes_lectura = read(descriptor_origen, buffer, 1);

        if(bytes_lectura == -1 || (buffer[0] < 32) || (buffer[0] > 127))//revisar que leyo un caracter leible
            cont++;
        else if(bytes_lectura == 0)
            break;
        i++;
    }

    if(cont == 5)
        printf("ADVERTENCIA: Se encontraron characteres poco comunes, posiblemente ingreso una clave incorecta\n");

    close(descriptor_origen);
}