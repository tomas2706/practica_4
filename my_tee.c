#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 1 //Quantitat de bytes que llegirem i escriurem a cada operació.

int main(int argc, char *argv[]) {
    if (argc != 2) { //Comprovem que només un argument (nom del fitxer).
        fprintf(stderr, "Ús: %s <nom_fitxer>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        // Només escriure, crear el fitxer, esborra el contingut si el fitxer existeix, lectura/escriptura per al propietari, només lectura per altres usua>
    if (fd == -1) { // Si open falla, retorna -1, i utilitzem perror per mostrar l'error.
        perror("Error obrint el fitxer");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE]; // Emmagatzema les dades llegides temporalment.
    ssize_t bytes_read; // Guarda el nombre de bytes llegits a cada operació.

    while ((bytes_read = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0) { // Llegeix BUFFER_SIZE bytes des de l'entrada. Retorna 0 si no hi ha res més a lle>
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) { // Escriu les dades llegides amb el bytes_read.
            perror("Error escrivint a la sortida estàndard");
            close(fd);
            exit(EXIT_FAILURE);
        }

        if (write(fd, buffer, bytes_read) == -1) { // Escriu les dades al fitxer obert per amb open.
            perror("Error escrivint al fitxer");
            close(fd);
            exit(EXIT_FAILURE);
        }
    }
