#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include<fcntl.h>
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
    {
        if(argc == 5)
            encrypt(argv[2], argv[3], argv[4]);
        if(argc == 4)
        {
            char *clave_default = CLAVE_DAFAULT;
            encrypt(argv[2], argv[3], clave_default);
        }
    }
    else if (strcmp(argv[1], "-d") == 0)
    {
        if(argc == 5)
            decrypt(argv[2], argv[3], argv[4]);
        if(argc == 4)
        {
            char *clave_default = CLAVE_DAFAULT;
            decrypt(argv[2], argv[3], clave_default);
        }
    }
    else
        printf("Argumentos invalidos\n");

    return 0;
}

void encrypt(char* input_file_location, char* output_name, char* clave) 
{
    int descriptor_origen, descriptor_enc;
    uint8_t semi_clave=0, buffer;
    uint16_t temp;

    descriptor_origen = open(input_file_location, O_RDONLY);
    if(source_fd == -1) 
    {
        printf("Error al abrir el archivo de origen\n");
        exit(EXIT_FAILURE);
    }

    descriptor_enc = open(output_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error al crear el archivo destino\n");
        exit(EXIT_FAILURE);
    }

    temp = clave[1] | (clave[0]<<8); // tomar dos bytes de la clave
    temp = -temp;// invertir valor

    for(int i=0;i<16;i+=2)// generar semiclave
    {
        if((temp & (1<<i)) > 0)
            semi_clave |= (1<<(i/2));
    }

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        //fwrite(buffer, sizeof(char), bytes_read, stdout);
        if(bytes_read < BUFFER_SIZE)
            buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }

    /*{
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
        */
        /*
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
    }*/

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