#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <fcntl.h>

#define MAX_CLIENTES 128

struct InfoCliente{
    int fd;
    int sd;
};

int espera_evento(int sd, struct InfoCliente * array_clientes, int * array_listos, int * nueva_conexion){
    fd_set conjunto_r;
    fd_set conjunto_w;
    fd_set copia;
    int listos = sizeof(array_listos)/sizeof(array_listos[0]);
    int n_sd;

    nueva_conexion = 0;
    

    FD_ZERO(&conjunto);

    n_sd = accept(sd, (struct sockaddr *)&cli, &cli_len);
    if(n_sd < 0){
        return -1;
    }

    unsigned i = 0;
    while(array_clientes[i].sd != -1){
        if(array_clientes[i++].sd == n_sd){
            printf("%d ya esta conectado!\n", n_sd);
            return -1;
        }
    }
    
    if((array_clientes[i-1].sd) == -1){
        nueva_conexion = 1;
        array_listos[listos] = i-1;
        array_clientes[i-1].fd = creat(strcat("cliente",i-1), 0666);
        listos++;
    }

//    unsigned j = 0;
//    int maxfd = 0;
//    for(j = 0 ; j < listos ; j++){
//        FD_SET(array_clientes[array_listos[j]].sd, &conjunto);
//        maxfd = max(maxfd, array_clientes[array_listos[j]].sd);
//    }
//
//    int r = select(maxfd+1, &conjunto, sizeof(fd_set));
//    if(r < 0){
//        perror("select");
//        exit(1);
//    }else{
//        for(j = 0; j < listos ; j++){
//            if(FD_ISSET(array_clientes[array_listos[j]].sd), &conjunto){
//            
//            }
//        }
//    }

    return nueva_conexion;
}

int max(int a, int b){
    if(a < b){
        return b;
    }
    return a;
}
