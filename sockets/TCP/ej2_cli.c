#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define TAM 128
#define PUERTO 5193
#define DEF_IP "127.0.0.1"

int main(int argc, void * argv []){
    char c;
    char * ip;
    if(argc > 1){
        ip = argv[1];
    }else{
        ip = DEF_IP;
    }
    struct sockaddr_in serv;
    char buffer[TAM];
    uint32_t nip = inet_addr(ip);

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    memcpy(&serv.sin_addr, &nip, 4);

    int sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Fallo en crear socket.");
        exit(1);
    }

    if(connect(sd, (struct sockaddr *)&serv, sizeof(serv)) < 0){
        perror("Fallo en connect");
        exit(2);
    }

    int leidos = 0;
    int escritos = 0;
    escritos = write(sd, ip, strlen(ip));
    if(escritos < 0){
        perror("Fallo en enviar ip\n");
        exit(5);
    }
    leidos = read(sd,&c,1);
    if(leidos < 0){
        perror("Fallo en recibir respuesta\n");
        exit(6);
    }
    if(c == 'n'){
        printf("IP NO VALIDA\n");
        exit(0);
    }
    printf("Conexion establecida!!\n");
    escritos = 0;
    leidos = 0;
    do{
        leidos = read(0, buffer, TAM);
        buffer[leidos-1] = '\0';
        escritos = write(sd, buffer, leidos);
    }while(strcmp("fin", buffer) && (leidos > 0) && (escritos == leidos));

    close(sd);
    return 0;
}
