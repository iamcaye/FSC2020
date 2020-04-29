#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX 128
struct Estructura{
	int frecuencia;
	char palabra[MAX];
};
int main(int argc, void *argv[]){
	int fd = open("frecuencia.bin", O_RDONLY);
	struct Estructura lista[MAX];
	int i = -1;
	struct Estructura tam;
	printf("%d", sizeof(tam));
	do{
		i++;
		read(fd, &lista[i], sizeof(tam));

	}while(lista[i].frecuencia != -1);

	printf("hellooooo %s", lista[0].palabra);

	return 0;
}
