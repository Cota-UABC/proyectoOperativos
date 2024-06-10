#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

typedef struct PalabraCount //estructura para lista enlazada
{
    char palabra[50];
    int count;
    struct PalabraCount *next;
} PalabraCount;

void uso(void);
void lista(char*, int);
void agregar_incrementar(PalabraCount **head, const char *palabra);
void print_palabras(PalabraCount *head, int min_rep);
void free_palabra_lista(PalabraCount *head);


int main(int argc, char *argv[])
{
    if(argc != 4)// todo
    {
        printf("Parametros incorrectos\n");
        uso();
        return 1;
    }

    //parametros
    else if(strcmp(argv[1], "-a") == 0)
    {
        lista(argv[2], atoi(argv[3]));
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
    //
}

void lista(char* file_location, int min_rep)
{
    char buffer[BUFFER_SIZE], palabra[50];
    int bytes_read;

    PalabraCount *palabra_lista = NULL;//lista enlazada
    int palabra_index = 0;

    int descriptor = open(file_location, O_RDONLY);
    if (descriptor == -1) {
        perror("Error al abrir el archivo\n");
        close(descriptor);
        return;
    }

    //leer archivo
    while ((bytes_read = read(descriptor, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            //si encuentra un separador de palabras
            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r' || buffer[i] == '.' ) {
                if (palabra_index > 0) {
                    palabra[palabra_index] = '\0';
                    agregar_incrementar(&palabra_lista, palabra);
                    palabra_index = 0;
                }
            } else {
                if (palabra_index < 49) {
                    palabra[palabra_index++] = buffer[i];
                }
            }
        }
    }

    if (palabra_index > 0) {// contruir palabra faltante al final de lectura
        palabra[palabra_index] = '\0';
        agregar_incrementar(&palabra_lista, palabra);
    }

    close(descriptor);

    print_palabras(palabra_lista, min_rep);

    free_palabra_lista(palabra_lista);
}

void agregar_incrementar(PalabraCount **head, const char *palabra)//head es la dirreccion a puntero a la lista enlazada 
{
    PalabraCount *current = *head;
    PalabraCount *anter = NULL;

    while (current != NULL) 
    {
        if (strcmp(current->palabra, palabra) == 0) {
            current->count++;
            return;//return from function
        }
        anter = current;
        current = current->next;
    }

    //if current == null
    PalabraCount *nuevo_nodo = (PalabraCount *)malloc(sizeof(PalabraCount));
    strcpy(nuevo_nodo->palabra, palabra);
    nuevo_nodo->count = 1;
    nuevo_nodo->next = NULL;

    if (anter == NULL) //si es el primero creado
        *head = nuevo_nodo;
    else 
        anter->next = nuevo_nodo;
}

void print_palabras(PalabraCount *head, int min_rep) 
{
    PalabraCount *current = head;
    while (current != NULL) {
        if (current->count >= min_rep) {
            printf("%s: %d\n", current->palabra, current->count);
        }
        current = current->next;
    }
}

void free_palabra_lista(PalabraCount *head) 
{
    PalabraCount *current = head;
    while (current != NULL) {
        PalabraCount *next = current->next;
        free(current);
        current = next;
    }
}