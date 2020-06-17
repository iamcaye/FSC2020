#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_CLIENTES 1024

struct InfoCliente{
    int fd;
    int sd;
};

int espera_evento(int sd, struct InfoCliente * array_clientes, int * array_listos, int * nueva_conexion){
    fd_set conjunto;
    struct sockaddr_in * cli;
    socklen_t cli_len = sizeof(cli);
    int listos = 0;
    int n_sd;
    unsigned n= 0;
    unsigned i= 0;

    nueva_conexion = 0;

    FD_ZERO(&conjunto);
    while((n < MAX_CLIENTES) && *(array_listos+n)){
        FD_SET(array_clientes[*(array_listos+n)].sd, &conjunto);
    }

    n_sd = accept(sd, (struct sockaddr *)&cli, &cli_len);
    if(n_sd < 0){
        perror("accept");
        return -1;
    }

    if(FD_ISSET(n_sd, &conjunto)){
        printf("Conexion ya registrada(%d)\n", n_sd);
    }else{
        while(array_clientes[i].sd != -1){
            i++;
        }        
        if(i < MAX_CLIENTES){
            *nueva_conexion = 1;
            array_clientes[i].sd = n_sd;
        }
    }
    
    while(*(array_listos+listos)){
        listos++;
    }

    return listos-1;
}

int main(){return 0;}
