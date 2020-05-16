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
    if(argc < 2){
        printf("Uso: %s <Fichero>", argv[0]);
        exit(1);
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0){
        perror("Fallo en open");
        exit(1);
    }

    struct sockaddr_in serv;
    int sd;
    uint32_t ip_serv;

    ip_serv = inet_addr("127.0.0.1");
    if(ip_serv < 0){
        perror("Fallo en asignar la IP del servidor");
        close(fd);
        exit(2);
    }
    
    sd = socket(PF_INET, SOCK_DGRAM, 0);
    if(sd < 0){
        perror("Fallo en crear socket");
        close(fd);
        exit(3);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    memcpy(&serv.sin_addr, &ip_serv, sizeof(serv));

    char buffer[TAM];
    int leidos = 0;
    int enviados = 0;

    do{
        leidos = read(fd,buffer,TAM);
        buffer[leidos-1]='\0';
        enviados = sendto(
                sd,
                buffer,
                leidos,
                0,
                (struct sockaddr *)&serv,
                sizeof(serv));
    }while((leidos > 0) && (enviados == leidos));

    if(leidos != leidos){
        printf("leidos != 0\n");
    }

    close(sd);
    close(fd);

    return 0;
}
