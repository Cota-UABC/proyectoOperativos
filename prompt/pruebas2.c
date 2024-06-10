#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#define BUFFER_SIZE 30

int main() {
    int fd;
    uint8_t buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    
    // Abrir el archivo en modo lectura/escritura
    fd = open("texto.txt", O_RDWR);
    if (fd == -1) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }
    
    //for(int i=0;i<)
    /*if (lseek(fd, 10, SEEK_CUR) == (off_t) -1) { //SEEK_SET
        perror("Error en lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }*/
    
    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        //fwrite(buffer, sizeof(char), bytes_read, stdout);
        if(bytes_read < BUFFER_SIZE)
            buffer[bytes_read] = '\0';
        printf("%s", buffer);
    }
    printf("\n");
    
    // Cerrar el archivo
    close(fd);
    
    return 0;
}
