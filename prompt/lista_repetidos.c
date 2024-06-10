#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <stdint.h>
#include <unistd.h>

#define BUFFER_SIZE 8192

typedef struct PalabraCount //estructura para lista enlazada
{
    char palabra[50];
    int count;
    struct PalabraCount *next;
} PalabraCount;

void uso(void);
char* lista(char*, int);
void agregar_incrementar(PalabraCount **head, const char *palabra);
void print_palabras(PalabraCount *head, int min_rep);
void free_palabra_lista(PalabraCount *head);


int main(int argc, char *argv[])
{

    if(argc != 4 && argc != 5)
    {
        printf("Parametros incorrectos\n");
        uso();
        return 1;
    }

    //parametros
    else if( (strcmp(argv[1], "-p") == 0 & argc == 4) || (strcmp(argv[1], "-f") == 0 & argc == 5) ) 
    {   
        char buffer[BUFFER_SIZE];
        int pipefds[2];

        //crear entubamiento
        if(pipe(pipefds) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        //crear subproceso
        pid_t pid = fork();

        if (pid < 0) {// error
            printf("Error al crear un proceso hijo");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)// Proceso hijo
        {
            char* list = lista(argv[2], atoi(argv[3]));
            close(pipefds[0]);
            write(pipefds[1], list, strlen(list));
            close(pipefds[1]);
            return 0;
        } 
        else       
        {
            wait(NULL);// Proceso padre
            close(pipefds[1]);
            read(pipefds[0], buffer, sizeof(buffer));
            close(pipefds[0]);
            buffer[strlen(buffer)] = '\0';

            if(strcmp(argv[1], "-p") == 0)
                printf("%s", buffer);

            else if(strcmp(argv[1], "-f") == 0)
            {
                int descriptor = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (descriptor == -1) 
                {
                    printf("Error al crear el archivo destino\n");
                    close(descriptor);
                    exit(EXIT_FAILURE);
                }
                write(descriptor,buffer,strlen(buffer));
                printf("Archivo creado correctamente en: %s\n",argv[4]);
                close(descriptor);
            }
        }         

    }
    else
    {
        printf("Parametros invalidos\n");
        uso();
    }

    return 0;
}

void uso(void)
{
    printf("Uso: \n");
    printf("    lista_repetidos -p [RUTA ARCHIVO FUENTE] [NUMERO MINIMO REPETICIONES]\n");
    printf("    lista_repetidos -f [RUTA ARCHIVO FUENTE] [NUMERO MINIMO REPETICIONES] [RUTA ARCHIVO DE SALIDA]\n");
}

char* lista(char* file_location, int min_rep)
{
    char* buffer = (char*)malloc(BUFFER_SIZE * sizeof(char));
    char palabra[50];
    int bytes_read;

    PalabraCount *palabra_lista = NULL;//lista enlazada
    int palabra_index = 0;

    int descriptor = open(file_location, O_RDONLY);
    if (descriptor == -1) {
        perror("Error al abrir el archivo\n");
        close(descriptor);
        return 0;
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

    //print_palabras(palabra_lista, min_rep);

    
    bzero(buffer, sizeof(buffer));

    PalabraCount *current = palabra_lista;
    while (current != NULL) {
        if (current->count >= min_rep)
            sprintf(buffer, "%s%s: %d\n",buffer ,current->palabra, current->count);//concatenar cadena
        current = current->next;
    }

    free_palabra_lista(palabra_lista);

    return buffer;
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