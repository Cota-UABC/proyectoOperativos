#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 512
#define MAX_STDIN_BUFFER 512
#define MAX_COMMAND_LENGTH 256
#define MAX_USER_LENGTH 256


int main() 
{
    char ch;
    uint16_t i;
    uint8_t past_input_counter = 0;
    char input[MAX_INPUT_LENGTH], command[MAX_COMMAND_LENGTH], username[MAX_USER_LENGTH];

    struct passwd *pw = getpwuid(getuid());//obtener usuario?
    strcpy(username, pw->pw_name);

    while(1) 
    {
        printf("\033[33m%s\033[34m:ENCRYPT PROMPT>\033[0m", username);
        fflush(stdout);// para evitar errores de salida

        i=0;
        while ((ch = getchar()) != '\n')// stdin
        {
            input[i] = ch;
            i++;
        }
        input[i] = '\0';

        char *args[MAX_INPUT_LENGTH];// dividir en tokens
        char *token = strtok(input, " ");

        if(token == NULL || token[0] == '\n')//si solo se preciono enter           
            continue;
        else 
        {
            int i = 0;
            while (token != NULL) 
            {
                args[i] = token;
                i++;
                token = strtok(NULL, " ");//llamadas subsecuentes
            }
            args[i] = NULL;//terminacion de argumentos
        }

        if (strcmp(args[0], "exit") == 0) //salir del prompt
            break;

        // Verificar si el comando es _________
        if (strcmp(args[0], "encrypt") == 0) 
        {
            char full_path[MAX_INPUT_LENGTH];
            snprintf(full_path, sizeof(full_path), "./%s", args[0]);
            args[0] = full_path;
        }

        pid_t pid = fork();// crear subprocesos

        if (pid < 0) {// error
            perror("Error al crear un proceso hijo");
            exit(EXIT_FAILURE);
        } else if (pid == 0)// Proceso hijo
        {
            execvp(args[0], args);
            perror("Error al ejecutar el comando");
            break;
        } else            
            wait(NULL);// Proceso padre
    }


    return 0;
}