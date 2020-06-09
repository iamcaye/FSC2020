#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#define TAM 128

#define estadoA 1
#define estadoB 2

#define eventoA 1
#define eventoB 2
#define eventoT 3

bool timer = false;

int espera_evento(){
    int res;
    int leidos = 0;
    char c;
    leidos = read(0, &c, 1); /*lee un caracter del teclado*/
    printf("Que pasa aqui\n");

    if(leidos == 0){
        return eventoT;
    }
    char barraN;
    read(0,&barraN,1); /*para quitar el retorno de carro*/
    switch(c){
        case 'A':
            res = eventoA;
            break;
        case 'B':
            res = eventoB;
            break;
        default:
            break;
    }
    return res;
}


void manejador(int s){
    write(1, "alarma", strlen("alarma"));
    signal(SIGALRM, manejador);
}

int main(int argc , void * argv[]){
    int estado;
    int evento = 0;
    int fin = 0;
    int cont = 0;
    estado = estadoA;
    signal(SIGALRM, manejador);
    alarm(5);

    while(!fin){
        evento = espera_evento();
        switch(estado){
            case estadoA:
                if(evento == eventoA){
                    cont++;
                    if(cont == 3){
                        cont = 0;
                        estado = estadoB;
                        printf("EstadoA ---> EstadoB  (%d)\n", estado);
                    }
                }else if(evento == estadoB){
                    estado = estadoB;
                    printf("EstadoA ---> EstadoB  (%d)\n", estado);
                }else if(evento == eventoT){
                    estado = estadoB;
                    printf("EstadoA ---> EstadoB  (%d)\n", estado);
                    evento = 0;
                }
                break;
            case estadoB:
                if(evento == eventoB){
                    cont++;
                    if(cont == 4){
                        fin = 1;
                        printf("FIN\n");
                    }
                }else if(evento == eventoA){
                    estado = estadoA;
                    printf("EstadoB ---> EstadoA (%d)\n", estado);
                    alarm(5);
                    cont = 0;
                }
        }
    }
    return 0;
}
