#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pwd.h>
#include <stdint.h>

#define MAX_INPUT_LENGTH 512
#define MAX_ARGS 15
#define MAX_STDIN_BUFFER 512
#define MAX_USER_LENGTH 256
#define MAX_PATH_LENGTH 256

void uso(void);

int main() 
{
    char ch;
    char full_path[MAX_INPUT_LENGTH];
    uint16_t c,c2;
    uint8_t pipe_f = 0, num_arg_separador;
    int pipefd[2];
    char input[MAX_INPUT_LENGTH], username[MAX_USER_LENGTH], path[MAX_PATH_LENGTH], path_prompt[MAX_PATH_LENGTH];
    char *args[MAX_ARGS]= {NULL}, *args2[MAX_ARGS]= {NULL};

    struct passwd *pw = getpwuid(getuid());//obtener usuario
    strcpy(username, pw->pw_name);

    getcwd(path_prompt, sizeof(path_prompt));// obtener path del prompt

    while(1) 
    {
        getcwd(path, sizeof(path));// obtener path

        for(int i = 0; i < MAX_ARGS; i++)//limpiar args
        {
            args[i] = malloc(MAX_INPUT_LENGTH * 8);
            args2[i] = malloc(MAX_INPUT_LENGTH * 8);
        }

        printf("\033[33m%s\033[31m:%s>\033[0m", username, path);
        fflush(stdout);// para evitar errores de salida

        c=0;
        while ((ch = getchar()) != '\n')// stdin
        {
            input[c] = ch;
            c++;
        }
        input[c] = '\0';

        c = 0;// contador para cada palabra
        uint8_t j = 0; //contador para cada letra
        for(int i=0;i<(strlen(input)+1);i++)
        {
            if(input[i] != ' ' && input[i] != '\0' )
            {
                args[c][j] = input[i];
                j++;
                if(input[i] == '|')
                {
                    pipe_f = 1;
                    num_arg_separador = c;
                }
            }
            else
            {
                if(j != 0)
                {
                    args[c][j] = '\0';
                    c++;
                    j=0;
                }
            }
        }
        args[c] = NULL;

        if(args[0] == NULL)//si no hay argumentos
        {
            uso();
            continue;
        }
        
        /*for(int i=0;i<MAX_ARGS;i++)
        {
            if(args[i] == NULL)
                break;
            printf("%s ",args[i]);
        }

        printf("\n");*/

        if(strcmp(args[0], "exit") == 0) //salir del prompt
            break;

        // Verificar comandos propios
        if(strcmp(args[0], "encrypt") == 0) 
        {
            sprintf(full_path, "%s/%s", path_prompt, args[0]);
            args[0] = full_path;
        }
        else if(strcmp(args[0], "calcular") == 0)
        {
            sprintf(full_path, "%s/script_calcular/%s", path_prompt, args[0]);
            args[0] = full_path;
        }
        else if(strcmp(args[0], "borrar_antiguos") == 0)
        {
            sprintf(full_path, "%s/script_borrar/remove_month",path_prompt);
            args[0] = full_path;
        }
        else if(strcmp(args[0], "lista_repetidos") == 0) 
        {
            sprintf(full_path, "%s/%s", path_prompt, args[0]);
            args[0] = full_path;
        }
        else if(strcmp(args[0], "crear_archivos") == 0) 
        {
            sprintf(full_path, "%s/%s", path_prompt, args[0]);
            args[0] = full_path;
        }
        else if(strcmp(args[0], "regresar") == 0) 
        {
            chdir(path_prompt);
            continue;
        }

        pid_t pid = fork();// crear subproceso

        if (pid < 0) // error
        {
            printf("Error al crear un proceso hijo");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0)// Proceso hijo
        {
            if(strcmp(args[0], "cd") == 0)
            {
                chdir(args[1]);
            }
            else
            {
                if(pipe_f == 0)// no entubamiento
                {
                    execvp(args[0], args);
                    uso();
                    break;
                }
                else// procesar entubamiento
                {
                    //separa args
                    c=num_arg_separador+1;
                    c2=0;
                    while(args[c] != NULL)
                    {
                        args2[c2] = args[c];
                        c++;
                        c2++;
                    }
                    args2[c2] = NULL;
                    args[num_arg_separador] = NULL;

                    if (pipe(pipefd) == -1)// pipe
                    {
                        perror("pipe");
                        exit(EXIT_FAILURE);
                    }

                    pid_t pid2 = fork();// segundo fork

                    if (pid2 < 0) // error
                    {
                        printf("Error al crear un proceso hijo");
                        exit(EXIT_FAILURE);
                    } 
                    else if (pid2 == 0)// Proceso hijo 2
                    {
                        //printf("proceso hijo\n");
                        dup2(pipefd[1], STDOUT_FILENO);// redirigir salida a pipe
                        close(pipefd[0]);

                        execvp(args[0], args);
                        uso();
                        break;
                    }
                    else // Proceso padre
                    {
                        wait(NULL);
                        //printf("proceso padre\n");
                        dup2(pipefd[0], STDIN_FILENO);
                        close(pipefd[1]);
                    
                        execvp(args2[0], args);
                        uso();
                        break;
                    }
                }
            }
        } 
        else            
            wait(NULL);// Proceso padre

        pipe_f=0;
        
        
        for(int i = 0; i < MAX_ARGS; i++)//liberar args
        {
            if (args[i] != NULL) 
            {
                free(args[i]);
                args[i] = NULL;
            }
            if (args2[i] != NULL) 
            {
                free(args2[i]);
                args2[i] = NULL;
            }
        }
    }

    return 0;
}

void uso()
{
    printf("Error al ejecutar comando o no existe\n\n");
    printf("Comandos disponibles: \n");
    printf("    - \033[32mcalcular:\033[0m Calcula el tamaño en bytes de los archivos dentro de una carpeta\n");
    printf("    - \033[32mborrar_antiguos:\033[0m Borra los archivos con una antiguedad de mes\n");
    printf("    - \033[32mencrypt:\033[0m Encrypta o desencripta un archivo\n");
    printf("    - \033[32mlista_repetidos:\033[0m Muestra una lista con palabras repetidas de un archivo\n");
    printf("    - \033[32mregresar:\033[0m Regresa a la carpeta origen del prompt\n");
}