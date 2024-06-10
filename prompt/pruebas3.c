#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 10

int main() {
    int fd;
    ssize_t bytes_read;
    char buffer[BUFFER_SIZE];

    // Abrir el archivo en modo solo lectura
    fd = open("texto.txt", O_RDONLY);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    // Leer el contenido del archivo hasta el final
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        // Procesar los datos leídos
        fwrite(buffer, sizeof(char), bytes_read, stdout);
    }

    if (bytes_read == -1) {
        perror("Error al leer el archivo");
        close(fd);
        exit(EXIT_FAILURE);
    }

    // Cerrar el archivo
    close(fd);

    return 0;
}
