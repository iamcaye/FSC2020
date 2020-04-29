#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>

int main(int argc, void * argv[]){
	int n = 0;
	pid_t p;
	while(n < 10){
		p = fork();
		if(p < 0){
			perror("Falta de recursos para fork");
			exit(1);
		}else if(p == 0){
			printf("Soy el hijo %d. Mi padre ha creado %d hijos.\n", getpid(), n);
			exit(1);
		}else{
			n++;
		}
	}

	for(unsigned i = 0 ; i < 10 ; i++){
		wait(0);
	}

	return 0;
}
