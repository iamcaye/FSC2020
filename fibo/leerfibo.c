#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX 1000000

int main(int argc, void * argv[]){
	if(argc <2){
		printf("Uso: ./lF <N_fich>");
		exit(2);
	}

	long long list[MAX];
	int fd;
	if((fd = open(argv[1], O_RDONLY)) < 0){
		perror("Fallo en open");
		exit(1);
	}

	int leidos;
	if(leidos = read(fd,list,sizeof(list)) != sizeof(list)){
		perror("Fallo en leer");
		exit(1);
	}
	

	unsigned int i = 0;
	do{
		printf("<|%d -- %lld|>\n", i, list[i]);
		i++;
	}while(list[i] >= 0 && i < MAX);



	return 0;
}
