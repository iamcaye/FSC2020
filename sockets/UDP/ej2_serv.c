#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PUERTO 4950
#define TAM 128

int main(int argc, void *argv[]){
    int sd;
    int fd = 0;
    struct sockaddr_in serv;
    struct sockaddr cli;

    fd = creat("fichero-destino", 0666);
    if(fd < 0){
        perror("Fallo en open fichero-destino");
        exit(1);
    }

    sd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sd < 0){
        perror("Fallo en crear socket");
        close(fd);
        exit(2);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    serv.sin_addr.s_addr = INADDR_ANY;

    if(bind(sd, (struct sockaddr *)&serv, sizeof(serv)) < 0){
        perror("Fallo en vincular socket al servidor");
        close(fd);
        close(sd);
        exit(3);
    }

    char buffer[TAM];
    int leidos = 0;
    int escritos = 0;
    socklen_t ncli = sizeof(cli);

    printf("Listo para recibir mensajes\n");

    do{
        leidos = recvfrom(
                sd,
                buffer,
                TAM,
                0,
                (struct sockaddr *)&cli,
                &ncli); 

        escritos = write(fd,buffer,leidos);
    }while((leidos > 0) && (escritos == leidos));

    close(fd);
    close(sd);
    return 0;

}

