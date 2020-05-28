#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#define PUERTO 5193
#define TAM 128

int main(int argc, void * argv[]){
    struct sockaddr_in serv;
    struct sockaddr_in cli;
    char buffer[TAM] = {[0 ... TAM-1] = '\0'} ;

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    serv.sin_addr.s_addr = INADDR_ANY;

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("socket");
        exit(1);
    }

    socklen_t serv_len = sizeof(serv);
    if(bind(sd, (struct sockaddr *)&serv, serv_len) < 0){
        perror("bind");
        exit(2);
    }

    if(listen(sd, 10) < 0){
        perror("listen");
        exit(4);
    }

    int n_sd;
    int leidos = 0;
    int escrito = 0;    
    int fd;
    socklen_t addr_len = sizeof(cli);
    uint8_t length = 0;
    while(1){
        n_sd = accept(sd, (struct sockaddr *)&cli, &addr_len);
        if(n_sd < 0){
            perror("accept");
            break;
        }

        leidos = read(n_sd, &length, sizeof(uint8_t));
        if(leidos < 0){
            perror("read length");
            close(n_sd);
            break;
        }

        leidos = read(n_sd, buffer, length);
        if((leidos < 0) || (leidos != length)){
            perror("read name");
            close(n_sd);
            break;
        }

        fd = creat(buffer, 0666);
        if(fd < 0){
            perror("creat");
            close(n_sd);
            break;
        }

        do{
            leidos = read(n_sd, buffer, TAM);
            escrito = write(fd, buffer, leidos);
        }while((leidos > 0) && (escrito == leidos));

        if(leidos == escrito){
            escrito = write(n_sd, "y", 1);
        }else{
            escrito = write(n_sd, "n", 1);
        }
    }

    close(sd);
    return 0;
}
