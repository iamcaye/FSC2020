#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#define TAM 128

#define estadoA 1
#define estadoB 2

#define eventoA 1
#define eventoB 2

int espera_evento(){
    int res;
    char c;
    read(0, &c, 1); /*lee un caracter del teclado*/

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

int main(int argc , void * argv[]){
    int estado, evento;
    int fin = 0;
    int cont = 0;
    estado = estadoA;

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
                    cont = 0;
                }
        }
    }
    return 0;
}
