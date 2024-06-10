#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096

typedef struct WordCount //estructura para lista enlazada
{
    char word[50];
    int count;
    struct WordCount *next;
} WordCount;

void add_or_increment(WordCount **head, const char *word)//head es la dirreccion a puntero a la lista enlazada 
{
    WordCount *current = *head;
    WordCount *prev = NULL;

    while (current != NULL) 
    {
        if (strcmp(current->word, word) == 0) {
            current->count++;
            return;//return from function
        }
        prev = current;
        current = current->next;
    }

    //if current == null
    WordCount *new_node = (WordCount *)malloc(sizeof(WordCount));
    strcpy(new_node->word, word);
    new_node->count = 1;
    new_node->next = NULL;

    if (prev == NULL) //si es primero creado
        *head = new_node;
    else 
        prev->next = new_node;
}

void print_words(WordCount *head, int min_rep) 
{
    WordCount *current = head;
    while (current != NULL) {
        if (current->count >= min_rep) {
            printf("%s: %d\n", current->word, current->count);
        }
        current = current->next;
    }
}

void free_word_list(WordCount *head) 
{
    WordCount *current = head;
    while (current != NULL) {
        WordCount *next = current->next;
        free(current);
        current = next;
    }
}

void lista(char* file_location, int min_rep) {
    int fd = open(file_location, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return;
    }

    char buffer[BUFFER_SIZE];
    int bytes_read;
    WordCount *word_list = NULL;

    char word[50];
    int word_index = 0;

    while ((bytes_read = read(fd, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == ' ' || buffer[i] == '\n' || buffer[i] == '\t' || buffer[i] == '\r') {
                if (word_index > 0) {
                    word[word_index] = '\0';
                    add_or_increment(&word_list, word);
                    word_index = 0;
                }
            } else {
                if (word_index < 49) {
                    word[word_index++] = buffer[i];
                }
            }
        }
    }

    if (word_index > 0) {// contruir palabra faltante al final de lectura
        word[word_index] = '\0';
        add_or_increment(&word_list, word);
    }

    close(fd);

    print_words(word_list, min_rep);

    free_word_list(word_list);
}

int main() {
    char file_location[] = "./texto_para_lista.txt";
    int min_rep = 2;
    lista(file_location, min_rep);
    return 0;
}
