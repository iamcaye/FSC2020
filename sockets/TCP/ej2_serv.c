#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>

#define PUERTO 5193
#define TAM 128

int main(int argc, void * argv[]){
    struct sockaddr_in serv, cli;
    int sd;
    int n_sd;
    char buffer[TAM];

    signal(SIGCHLD, SIG_IGN);

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_port = htons(PUERTO);
    serv.sin_addr.s_addr = INADDR_ANY;

    sd = socket(PF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("Fallo en crear socket");
        exit(2);
    }

    socklen_t serv_len = sizeof(serv);
    if(bind(sd, (struct sockaddr *)&serv, serv_len)){
        perror("Fallo en enlazar socket y serv");
        exit(3);
    }
    
    if(listen(sd, 10)){
        perror("Error en listen");
        exit(4);
    }

    char ips[TAM][TAM] = {[0 ... TAM-1][0 ... TAM-1] = '\0'};
    char ip[TAM]= {[0 ... TAM-1] = '\0'};
    int nips = 0;
    int leidos = 0;
    int escritos = 0;
    socklen_t addr_len;
    while(1){
        addr_len = sizeof(cli);
        memset(&cli, 0, sizeof(cli));
        n_sd = accept(sd, (struct sockaddr *)&cli, &addr_len);
        if(n_sd < 0){
            perror("Fallo en accept");
            exit(5);
        }else if(n_sd > 0){
            leidos = read(n_sd, ip, TAM);
            unsigned i = 0;
            while((i < nips) && strcmp(ips[i],ip)){
                //printf("%s sisisi %s\n", ip,ips[i]);
                i++;
            }
            if(i == nips){
                strcpy(ips[nips], ip);
                nips++;
                printf("Usuario conectado ->\t%s\n",ip);
                if(write(n_sd, "y", 1) != 1){
                    perror("Fallo en enviar respuesta\n");
                    exit(5);
                }
                pid_t p = fork();
                if(p < 0){
                    perror("fork");
                    exit(6);
                }else if(p == 0){
                    leidos = 0;
                    do{
                        leidos = read(n_sd, buffer, TAM);
                        write(1, ip, strlen(ip));
                        write(1, "\n", 2);
                        escritos = write(1, buffer, leidos);
                        write(1, "\n\n", 3);
                    }while((leidos > 0) && strcmp("fin", buffer) && (escritos == leidos));
                    close(n_sd);
                    exit(9);
                }
            }else{
                printf("(%s)IP no valida...\n", ip);
                if(write(n_sd, "n", 1) != 1){
                    perror("Fallo en enviar respuesta negativa");
                    exit(6);
                }
                close(n_sd);
            }
        }
    }

    close(sd);

    return 0;
}
