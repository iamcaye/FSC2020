#include <stdio.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

#define MAX 128

void manejador(int s){
    write(1, "Usuario conectado!\n", 20);
    signal(SIGUSR1, manejador);
}

int main(){
    int fd = creat("pid_chat", 0660);
    if(fd < 0){
        perror("Fallo en open fichero pid");
        exit(1);
    }
    pid_t padre = getpid();
    if(write(fd, &padre , sizeof(pid_t)) < 0){
        perror("Fallo en escribir pid");
        exit(1);
    }

    signal(SIGUSR1, manejador);

    fd = open("/tmp/fsc_chat", O_RDONLY);

    if(fd < 0){
        perror("Fallo en open fifo");
        exit(1);
    }

    fd_set conjunto, copia;

    FD_ZERO(&conjunto);

    FD_SET(fd, &conjunto);

    int leer =0;
    char buffer[MAX];

    while(1){
    
        memcpy(&copia, &conjunto, sizeof(fd_set));

        int r = select(fd+1, &copia, NULL, NULL, NULL);

        if(r < 0){
            perror("Fallo en select");
            exit(1);
        }else if(r > 0){
            if(FD_ISSET(fd, &copia)){
                leer = read(fd, buffer, MAX);
                if(leer > 0){
                    buffer[leer] = '\0';
                    printf("Recibido: %s", buffer);
                }else if(leer == 0){
                    if(errno != 0 && errno != EINTR){
                        printf("%d", errno);
                        perror("Fallo en read de la fifo");
                        exit(1);                
                    }else{
                        write(1, "Usuario desconectado...\n", 25);
                        pause();
                    }
                }
            }
        }
    }

    return 0;
}
