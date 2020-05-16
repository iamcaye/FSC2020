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

int main(int argc, void *argv[]){
    int fd = 0;
    struct sockaddr_in serv;
    uint32_t ip_serv;

    ip_serv = inet_addr("127.0.0.1");
    if(ip_serv > 0){
        perror("Fallo en obtener la IP del servidor");
        exit(2);
    }

    fd = socket(PF_INET, SOCK_DGRAM, 0);
    if(fd < 0){
        perror("Fallo en crear el socke");
        exit(3);
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    memcpy(&serv.sin_addr, &ip_serv, 4);

    char buffer[TAM];
    int leidos = 0;
    int env = 0;
    printf("Listo para enviar mensajes...\n");
    do{
        leidos = read(0, buffer, TAM);
        buffer[leidos-1]='\0';
        env = sendto(
                fd,
                buffer,
                leidos,
                0,
                (struct sockaddr *)&serv,
                sizeof(serv));
    }while(strcmp("fin", buffer) && (leidos > 0) && (env > 0));

    close(fd);

    return 0;
}
