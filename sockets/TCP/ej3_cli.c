#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define PUERTO 5193

int main(int argc, void * argv[]){
    uint32_t ipserv;
    char * fichero;
    if(argc == 1){
        printf("Uso <Optional ip> <Destino> <Origen>");
        exit(1);
    }else if(argc > 2){
        ipserv = inet_addr("127.0.0.1");
        fichero = argv[1];
    }else{
        ipserv = inet_addr(argv[1]);
        fichero = argv[2];
    }
    struct sockaddr_in serv;
    
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    memcpy(&serv.sin_addr, &ipserv, 4);

    int sd = socket(PF_INET, SOCK_STREAM,0);
    if(sd < 0){
        perror("Fallo en socket");
        exit(1);
    }

    socklen_t serv_len = sizeof(serv);
    if(connect(sd, (struct sockaddr *)&serv, serv_len) < 0){
        perror("connect");
        exit(2);
    }

    uint8_t length = strlen(ip); 
    int leidos = write(sd, &length, sizeof(uint8_t));
    if(leidos < 0){
        perror("write length");
        close(sd);
        exit(3);
    }
    leidos = write(sd, fichero, strlen(fichero));
    if(leidos < 0){
        perror("write name");
        close(sd);
        exit(3);
    }

    char c;
    leidos = read(sd, &c,1);
    if(leidos != 1){
        perror("write");
        close(sd);
        exit(3);
    }
    
    if(c == 'y'){
        printf("Fichero copiado con exito!!\n");
    }else{
        printf("Se han producido errores al hacer la copia...\n");
    }

    return 0;
}


