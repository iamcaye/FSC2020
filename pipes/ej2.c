#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MAX 128

void manejador(int s){
	wait(0);
	signal(SIGCLD,manejador);
}

int main(int argc, void * argv[]){
	if(argc < 2){
		printf("Uso: %s <N_caracteres>\n", argv[0]);
		exit(1);
	}

	signal(SIGCLD,manejador);

	pid_t p;
	int fd_envio[2];
	int fd_resp[2];

	char buffer[MAX];

	if(pipe(fd_envio) < 0){
		perror("Fallo en creacion de las pipe");
		exit(1);
	}

	if(pipe(fd_resp) < 0){
		perror("Fallo en creacion de las pipe");
		exit(1);
	}

	p = fork();
	if(p < 0){
		perror("Falta de recursos en para fork");
		exit(1);
	}else if(p == 0){ 			/* Codigo HIJO */
		close(fd_envio[1]);
		close(fd_resp[0]);

		int leidos = 0, cant = 0;
		char c = 'y';
		do{
			leidos = read(fd_envio[0], buffer, MAX);
			write(1,buffer,leidos);
			printf("\n");

			cant += leidos;
			if(cant > atoi(argv[1])){
				c = 'n';
				printf("Limite\n");
			}
			write(fd_resp[1], &c, 1);

		}while((leidos > 0) && (strcmp(buffer, "fin")) && (c != 'n'));


		close(fd_envio[0]);
		close(fd_resp[1]);	
		exit(1);

	}else{					/* Codigo PADRE */
		close(fd_envio[0]);
		close(fd_resp[1]);

		int escrito = 0;
		char v;
		do{
			escrito = read(0, buffer, MAX);
			buffer[escrito -1] = '\0';

			if(write(fd_envio[1], buffer,escrito) < 0){
				perror("Fallo en envio del padre");
				exit(1);
			}
			read(fd_resp[0], &v, 1);
		}while((escrito > 0) && (strcmp(buffer,"fin")) && v != 'n');

		close(fd_envio[1]);
		close(fd_resp[0]);
	}

	return 0;
}
