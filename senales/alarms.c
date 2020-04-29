#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>

void manejador(){
	write(1, "ALARMA!", 8);
	signal(SIGINT, SIG_DFL);
}

int main(int argc, void * argv[]){
	if(argc < 2){
		printf("Uso: %s <N_Segs>", argv[0]);
		exit(1);
	}
	signal(SIGINT, SIG_IGN);
	signal(SIGALRM, manejador);

	alarm(atoi(argv[1]));

	while(1);

	return 0;
}
