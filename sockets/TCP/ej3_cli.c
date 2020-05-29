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
#define TAM 128

int main(int argc, void * argv[]){
    uint32_t ipserv;
    char * fichero;
    char * origen;
    if(argc == 1){
        printf("Uso <Optional ip> <Destino> <Origen>");
        exit(1);
    }else if(argc > 2){
        ipserv = inet_addr("127.0.0.1");
        fichero = argv[1];
        origen = argv[2];
    }else{
        ipserv = inet_addr(argv[1]);
        fichero = argv[2];
        origen = argv[3];
    }

    char buffer[TAM] = {[0 ... TAM-1] = '\0'};
    struct sockaddr_in serv;
    int sd, fd, leidos, escritos;
    socklen_t serv_len;
    uint8_t length;
    char c;
    
    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    memcpy(&serv.sin_addr, &ipserv, 4);

    sd = socket(PF_INET, SOCK_STREAM,0);
    if(sd < 0){
        perror("Fallo en socket");
        exit(1);
    }

    serv_len = sizeof(serv);
    if(connect(sd, (struct sockaddr *)&serv, serv_len) < 0){
        perror("connect");
        exit(2);
    }
    printf("Conexion establecida con el servidor\n");

    fd = open(origen, O_RDONLY);
    if(fd < 0){
        perror("open origen");
        close(sd);
        exit(3);
    }

    length = strlen(fichero); 
    leidos = write(sd, &length, sizeof(uint8_t));
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

    printf("Inicio de transmision del fichero\n");
    do{
        leidos = read(fd, buffer, TAM);
        escritos = write(sd, buffer, leidos);
    }while((leidos > 0) && (escritos == leidos));

    if((leidos == 0) && (escritos == 0)){
        printf("Exito!\n"); 
    }else{
        printf("Fracaso\n");
    }

    close(fd);
    close(sd);;
    return 0;
}


