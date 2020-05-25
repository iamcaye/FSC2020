#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdlib.h>

#define PUERTO 4950
#define TAM 128

int main(int argc, void * argv[]){
    int fd = 0;
    struct sockaddr_in cli;
    struct sockaddr_in serv;

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("Fallo en crear socket");
        exit(1);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    serv.sin_addr.s_addr = INADDR_ANY;

    if(bind(fd, (struct sockaddr *)&serv, sizeof(serv)) < 0){
        perror("Fallo en bind");
        exit(2);
    }

    int leidos = 0;
    char buffer[TAM];
    socklen_t ncli = sizeof(cli);
    printf("Listo para recibir mensajes\n");

    do{
        leidos = recvfrom(fd,
                buffer,
                TAM,
                0,
                (struct sockaddr *)&cli,
                &ncli);

        buffer[leidos-1]='\0';
        printf("Recibido: %s\n", buffer);
    }while(strcmp(buffer,"fin") && (leidos > 0));

    close(fd);
    return 0;
}
