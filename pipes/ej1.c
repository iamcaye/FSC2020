#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

#define MAX 128

void manejador(int s){
	wait(0);
	signal(SIGCLD,manejador);
}

int main(int argc, void *argb[]){
	pid_t p;
	char buffer[MAX];
	int fd_pipe[2];

	signal(SIGCLD,manejador); /* Manejador de señal que ejecuta el hijo al morir para que el padre lo espere */

	if(pipe(fd_pipe) < 0){
		perror("Fallo en crear pipe");
		exit(1);
	}

	p = fork();
	if(p < 0){			
		perror("Falta de recursos para fork");
		exit(1);
	}else if(p == 0){		/* Codigo hijo */
		close(fd_pipe[1]);

		int leidos;
		do{
			if((leidos = read(fd_pipe[0], buffer, MAX)) < 0){
				perror("Fallo en read del hijo");
				exit(1);
			}
			write(1, buffer, leidos);
			printf("\n");
		}while((leidos > 0) && strcmp(buffer,"fin"));
		close(fd_pipe[0]);
		exit(0);
	}else{				/* Codigo padre */
		close(fd_pipe[0]);
		int leer = 0;
		do{
			leer = read(0, buffer,MAX);
			buffer[leer -1] = '\0';
			write(fd_pipe[1],buffer,leer);
		}while((leer > 0) && strcmp(buffer,"fin"));
		close(fd_pipe[1]);
	}

	return 0;
}
