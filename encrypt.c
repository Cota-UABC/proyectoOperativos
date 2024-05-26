#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <stdint.h>

#define KEY 36

void encrypt(FILE *input_file, FILE *output_file);

int main(int argc, char *argv[])
{
    if (argc > 4) {
        printf("Demasiadados argumentos\n");
        return 1;
    }

    //printf("Hola\n");

    FILE *input_file = fopen(argv[2], "rb");
    if (input_file == NULL) {
        perror("Error al abrir el archivo de entrada");
        return 1;
    }

    FILE *output_file = fopen(argv[3], "wb");
    if (output_file == NULL) {
        perror("Error al crear el archivo de salida");
        fclose(input_file);
        return 1;
    }

    encrypt(input_file, output_file);

    printf("Archivo encriptado correctamente como %s\n",argv[3]);

    fclose(input_file);
    fclose(output_file);

    return 0;
}

void encrypt(FILE *input_file, FILE *output_file) {
    uint8_t ch;
    int i = 0;
    while ((ch = fgetc(input_file)) != EOF && i < 50) {//while not in End OF File
        fputc(ch ^ KEY, output_file); // Operación XOR con la clave
        printf("%c", ch);
        i++;
    }
}