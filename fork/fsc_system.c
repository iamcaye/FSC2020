#include <stdio.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, void *argv[]){

	if(argc < 2){
		printf("Uso: %s <Comando>", argv[0]);
		exit(1);
	}

	char buffer[128];
	strcpy(buffer, argv[1]);
	for(unsigned i = 2 ; i < argc ; i++){
		strcat(buffer, " ");
		strcat(buffer, argv[i]);
	}
	pid_t p = fork();

	if(p < 0){
		perror("Falta de recursos para ejecutar fork");
		exit(0);
	}else if(p == 0){
		int r = execl("/bin/sh", "sh", "-c", buffer, NULL);
		exit(1);
	}else{
		wait(0);
	}

	return 0;
}
