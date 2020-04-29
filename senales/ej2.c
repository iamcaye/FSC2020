#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <fcntl.h>

typedef void (*sighandler_t)(int);
sighandler_t puntero_a_funcion;

void manejador2(int s){
	write(1, "manejador2\n", 11);
}

void manejador1(int s){
	write(1, "manejador1\n", 11);
	puntero_a_funcion = signal(SIGINT, manejador2);
}

int main(){
	puntero_a_funcion = signal(SIGINT, manejador1);

	while(1);

	return 0;
}
