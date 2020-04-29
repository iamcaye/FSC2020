#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>

#define MAX 1000000


int fib_rec(int n);
int global = 0;

void timer(int s){
	global = 1;
}

int main(int argc, void * argv[]){
	if(argc < 3){
		printf("Usage ./gFr <time> <ficheroDestino>");
		exit(1);
	}

	signal(SIGALRM, timer);

	long long list[MAX] ={[0 ... MAX-1] = -1};
	unsigned long i = 0;

	alarm(atoi(argv[1]));
	while(i < MAX && global == 0){
		list[i] = (long long)fib_rec(i);
		printf("<|%ld -- %lld|>\t", i, list[i]);
		i++;
	}

	int fd;
	if((fd = creat(argv[2], 0666)) < 0){
		perror("Fallo en open");
		exit(1);
	}

	int leidos;
	if((leidos = write(fd,list,sizeof(list))) < 0){
		perror("Fallo en write");
		exit(1);
	}
//	for(unsigned h = 0 ; h < 10 ; h++){
//		printf("<|%ld -- %lld|>\t", h, list[i]);
//	}


	close(fd);
	return 0;
}

int fib_rec(int n) {
    if (n < 2) {
        return n;
    } else {
        return fib_rec(n-1) + fib_rec(n-2);
    }
}
