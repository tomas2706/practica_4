#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h> // Per obrir fitxers i utilitzar constants relacionades.
#include <unistd.h> // Serveix per coses com el read i el close.

#define BUFFER_SIZE 1

int main(int argc, char *argv[]) {
    if (argc != 3) { // Comprova que hi hagi dos arguments (els noms dels fitxers que s'han de comparar).
        fprintf(stderr, "Ús: %s <fitxer1> <fitxer2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd1 = open(argv[1], O_RDONLY); // Obre els fitxers en mode només lectura.
    if (fd1 == -1) {
        perror("Error obrint el primer fitxer"); // Mostra l'error si es produeix, i retorna -1.
        exit(EXIT_FAILURE);
    }

    int fd2 = open(argv[2], O_RDONLY); // Si el segon open falla es tanca primer el primer arxiu i després mostra l'error.
    if (fd2 == -1) {
        perror("Error obrint el segon fitxer");
        close(fd1);
        exit(EXIT_FAILURE);
    }

    char buf1[BUFFER_SIZE], buf2[BUFFER_SIZE]; // Buf1 i Buf2 són 2 arrays que emmagatzemen els bytes llegits a cada fitxer. (Són de mida d'1 byte).
    ssize_t bytes_read1, bytes_read2; // Contenen el nombre de bytes llegits per cada fitxer.
    int byte_pos = 1; // Compta la posició del byte que s'està comparant.
    int line = 1; // Compta el nombre de línies per a detectar salts de línia.

    while ((bytes_read1 = read(fd1, buf1, BUFFER_SIZE)) > 0 &&
           (bytes_read2 = read(fd2, buf2, BUFFER_SIZE)) > 0) { // Es llegeix un byte de cada arxiu alhora. El loop seguirà mentres no es produeixi cap error.
        if (buf1[0] != buf2[0]) {
            printf("Diferència trobada al byte %d, línia %d\n", byte_pos, line); // Si el byte de Buf1 és diferent al de Buf2, detecta la diferència i mostra la línia i el byte on està aquesta diferència.
            close(fd1); // Tanca el primer arxiu.
            close(fd2); // Tanca el segon arxiu.
            exit(EXIT_SUCCESS);
        }

        if (buf1[0] == '\n') { // Si hi ha un salt de línia, afegeix un al sumatori de línies de l'arxiu.
            line++;
        }

        byte_pos++; // Després de cada iteració, com que treballa amb un byte a cada cop, se suma un al comptador de bytes.
    }

    if (bytes_read1 != bytes_read2) { // Compara la longitud dels arxius.
        printf("Els fitxers tenen diferents longituds\n");
    } else {
        printf("Els fitxers són iguals\n");
    }

    if (close(fd1) == -1) { // Tanca el fitxer i en cas d'error el mostra.
        perror("Error tancant el primer fitxer");
    }

    if (close(fd2) == -1) { // Tanca el fitxer i en cas d'error el mostra.
        perror("Error tancant el segon fitxer");
    }

    return 0;
}
