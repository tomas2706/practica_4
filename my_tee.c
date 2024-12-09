#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	int fd = open(argv[1], O_CREAT, O_RDWR);
	if (fd == -1) {
        	perror("L'arxiu no s'ha pogut crear.");
        	return 1;
    	}
	close(fd);
}
