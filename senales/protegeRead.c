#include <stdio.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>

#define MAX 128

int main(){
	signal(SIGINT, SIG_IGN);

	char buffer[MAX] = {[0 ... MAX-1] = '\0'};

	int leidos;

	while((leidos >= 0) && (strcmp("fin", buffer)) || errno == EINTR){
		leidos = read(0, buffer, MAX-1);
		buffer[leidos] = "\0";
		write(1, buffer, leidos);
	}

}
