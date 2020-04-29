#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 128


int main(int argc, void *argv[]){

	if(argc < 2){
		printf("Uso: %s <N_fichero>\n", argv[0]);
		exit(1);
	}
	

	char buffer[MAX] = {[0 ... MAX-1] = '\0'};
	int fd_pipe[2];
	if(pipe(fd_pipe) < 0 ){
		perror("Fallo en abrir pipe");
		exit(1);
	}

	pid_t p = fork();
	if(p < 0){
		perror("Falta de recursos para fork");
		exit(1);
	}else if(p == 0){

		close(fd_pipe[1]);
		char res[MAX] = {[0 ... MAX-1] = '\0'};
		int leidos = 0;
		int escritos = 0;
		int fd = 0;
		
		leidos = read(fd_pipe[0], buffer, MAX);
		if((fd = open(buffer, O_WRONLY)) < 0){
			perror("fallo en read direccion destino de hijo");
			exit(1);
		}

		do{
			leidos = read(fd_pipe[0], buffer, MAX);
			for(unsigned i = 0 ; i < leidos ; i++){
				res[i] = tolower((unsigned char)buffer[i]);
			}

//			write(1, res,leidos);
			escritos = write(fd, res, leidos);
		}while((leidos > 1)/* && (escritos == leidos)*/);
		printf("fin hijo\n");
		close(fd_pipe[0]);
		close(fd);
		exit(1);

	}else{
		close(fd_pipe[0]);
		int leer = 0;
		int escribir = 0;
		

		int fd = open(argv[1], O_RDONLY);
		
		printf("Fichero de salida:\n");
		leer = read(0, buffer, MAX);
		buffer[leer-1] = '\0';

		escribir = write(fd_pipe[1], buffer, leer);

		do{
			leer = read(fd, buffer, MAX);
			escribir = write(fd_pipe[1], buffer, leer);
		}while((leer > 0) && (escribir == leer));
		close(fd_pipe[1]);
		wait(0);
		printf("fin padre\n");
	}
	return 0;
}
