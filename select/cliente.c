#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>


#define MAX 128

int main(){
    int fd = open("pid_chat", O_RDONLY);
    if(fd < 0){
        perror("Fallo en open fichero pid");
        exit(1);
    }

    pid_t padre;
    if(read(fd, &padre, sizeof(pid_t)) < 0){
        perror("Fallo en read fichero pid");
        exit(1);
    }

    kill(padre, SIGUSR1);

    int fd_fifo = open("/tmp/fsc_chat", O_WRONLY);

    fd_set conjunto, copia;

    FD_ZERO(&conjunto);

    FD_SET(0,&conjunto);
    FD_SET(fd_fifo, &conjunto);

    char buffer[MAX] = {[0 ... MAX-1] = '\0'};

    int leer = 0;
    while(1){
        leer = read(0, buffer, MAX);
        if(leer < 0){
            perror("Fallo en leer por teclado");
            exit(1);
        }else if(leer > 0){
            write(fd_fifo, buffer, leer);
        }else{
            break;
        }
    
    }

    return 0;
}
