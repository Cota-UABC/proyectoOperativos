#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1024

int main() {
    int source_fd, dest_fd; // Descriptores de archivo
    ssize_t bytes_read, bytes_written;
    char buffer[BUFFER_SIZE];

    // Abrir el archivo de origen
    source_fd = open("input.txt", O_RDONLY);
    if (source_fd == -1) {
        perror("Error al abrir el archivo de origen");
        exit(EXIT_FAILURE);
    }

    // Crear el archivo de destino
    dest_fd = open("copia_input.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1) {
        perror("Error al crear el archivo de destino");
        exit(EXIT_FAILURE);
    }

    // Leer del archivo de origen y escribir en el archivo de destino
    while ((bytes_read = read(source_fd, buffer, BUFFER_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error al escribir en el archivo de destino");
            exit(EXIT_FAILURE);
        }
    }

    // Comprobar errores de lectura
    if (bytes_read == -1) {
        perror("Error al leer del archivo de origen");
        exit(EXIT_FAILURE);
    }

    // Cerrar archivos
    if (close(source_fd) == -1) {
        perror("Error al cerrar el archivo de origen");
        exit(EXIT_FAILURE);
    }
    if (close(dest_fd) == -1) {
        perror("Error al cerrar el archivo de destino");
        exit(EXIT_FAILURE);
    }

    printf("Copia del archivo creada exitosamente.\n");
    return 0;
}
