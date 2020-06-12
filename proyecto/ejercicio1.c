#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_CLIENTES 128

struct InfoCliente{
    int fd;
    int sd;
};

int espera_evento(int sd, struct InfoCliente * array_clientes, int * array_listos, int * nueva_conexion){
    struct sockaddr_in * cli;
    socklen_t cli_len = sizeof(cli);
    int listos = sizeof(array_listos)/sizeof(array_listos[0]);
    int n_sd;

    nueva_conexion = 0;

    n_sd = accept(sd, (struct sockaddr *)&cli, &cli_len);
    if(n_sd < 0){
        perror("accept");
        return -1;
    }

    unsigned i = 0;
    while(array_clientes[i].sd != -1){
        if(array_clientes[i].sd == n_sd){
            printf("%d ya esta conectado!\n", n_sd);
            return -1;
        }
        i++;
    }
    
    if((array_clientes[i-1].sd) == -1){
        *nueva_conexion = 1;
        char * num;
        array_listos[listos] = i-1;
        sprintf(num, "%d", i-1);
        array_clientes[i-1].fd = creat(strcat("cliente",num), 0666);
        listos++;
    }

    return *nueva_conexion;
}

int main(){return 0;}
