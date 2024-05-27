#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdint.h>

#define CLAVE_DAFAULT "9c2[v:a$v/!0SCq?w2"
#define MAX_NAME_LENGHT 256

void encrypt(char*, char*, char*);
void decrypt(char*, char*, char*);

int main(int argc, char *argv[])
{
    if (argc > 5) {
        printf("Demasiadados argumentos\n");
        return 1;
    }

    //parametros
    else if (strcmp(argv[1], "-e") == 0)
        if(argc == 5)
            encrypt(argv[2], argv[3], argv[4]);
        if(argc == 4)
        {
            char *clave_default = CLAVE_DAFAULT;
            encrypt(argv[2], argv[3], clave_default);
        }
    else if (strcmp(argv[1], "-d") == 0)
        if(argc == 5)
            decrypt(argv[2], argv[3], argv[4]);
        if(argc == 4)
        {
            char *clave_default = CLAVE_DAFAULT;
            decrypt(argv[2], argv[3], clave_default);
        }
    else
        printf("Argumentos invalidos\n");

    return 0;
}

void encrypt(char* input_file_location, char* output_name, char* clave) 
{
    //char output_name[MAX_NAME_LENGHT];

    FILE *input_file = fopen(input_file_location, "rb");// open file
    if (input_file == NULL) 
    {
        perror("Error al abrir el archivo de entrada");
        return;
    }

    FILE *output_file = fopen(output_name, "wb");// crear output
    if (output_file == NULL) 
    {
        perror("Error al crear el archivo de salida");
        fclose(input_file);
        return;
    }


    int i = 0;
    uint8_t ch;
    while(1) 
    {
        ch = fgetc(input_file);//get byte
        if(feof(input_file))// break if end of file
            break;

        fputc(ch ^ clave[i], output_file); // Operación XOR con la clave

        i++;
        if(i == strlen(clave))// repetir la clave en ciclos
            i = 0;
    }

    printf("Archivo encriptado correctamente como %s\n",output_name);

    fclose(input_file);
    fclose(output_file);
}

void decrypt(char* input_file_location, char* output_name, char* clave)
{
    FILE *input_file = fopen(input_file_location, "rb");// open file
    if (input_file == NULL) 
    {
        perror("Error al abrir el archivo de entrada");
        return;
    }

    FILE *output_file = fopen(output_name, "wb");// crear output
    if (output_file == NULL) 
    {
        perror("Error al crear el archivo de salida");
        fclose(input_file);
        return;
    }

    int i = 0;
    uint8_t ch;
    while(1) 
    {
        ch = fgetc(input_file);//get byte
        if(feof(input_file))// break if end of file
            break;

        fputc(ch ^ clave[i], output_file); // Operación XOR con la clave

        i++;
        if(i == strlen(clave))// repetir la clave en ciclos
            i = 0;
    }

     printf("Archivo decriptado como %s\n",output_name);

    fclose(input_file);
    fclose(output_file);
}