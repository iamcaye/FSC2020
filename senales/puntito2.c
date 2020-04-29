#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>

void manejador(int s){
	signal(SIGALRM, manejador);
}

int main(){
	signal(SIGINT, SIG_IGN);
	signal(SIGALRM, manejador);

	struct timeval tv;
	struct itimerval timer;

	tv.tv_sec = 3;
	tv.tv_usec = 3000;

	timer.it_value = tv;
	timer.it_interval = tv;

	setitimer(ITIMER_REAL, &timer, NULL);

	while(1){
		pause();
		write(1, ".\n", 3);
	}

	return 0;
}
