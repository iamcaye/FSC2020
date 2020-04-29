#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define MAX 128

int main(int argc, void *argv[]){
	if(argc < 2){
		printf("Uso: %s <Fifo_path>\n", argv[0]);
		exit(1);
	}

	char buffer[MAX];
	int fd = open(argv[1], O_RDONLY);
	if(fd < 0){
		perror("Fallo en open fifo");
		exit(0);
	}

	int leidos;
	do{
		leidos = read(fd, buffer, MAX);
		write(1, buffer, leidos);
		printf("\n");
	}while((leidos > 0) && (strcmp(buffer, "fin")));
	return 0;
}
