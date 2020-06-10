#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

#define TAM 128

#define EST_OCIOSO 1
#define EST_FILTRANDO 2
#define BLOQUEO 3

#define ev_on 1
#define ev_off 2
#define ev_fin 3
#define ev_taco 4
#define ev_msj 5


int espera_evento(const char * buffer){
    int evento = 0;

    if(!strcmp("/on", buffer)){
        evento = ev_on;
    }else if(!strcmp("/off", buffer)){
        evento = ev_off;
    }else if(!strcmp("/fin", buffer)){
        evento = ev_fin;
    }else if(strstr(buffer, "taco") != NULL){
        evento = ev_taco;
    }else{
        evento = ev_msj;
    }

    return evento;
}


int main(int argc, void *argv []){
    int estado = EST_OCIOSO;
    int evento = 0;
    unsigned i = 0;
    int n = 0;
    char buffer[TAM] = {[0 ... TAM-1] = '\0'};
    evento = 0;
    bool fin = false;
    int leidos = 0;
    char plb[TAM];
    unsigned cont_tacos = 0;

    while(!fin){
        int leidos = read(0, buffer,TAM);
        if(leidos < 0){
            perror("read espera_evento");
            exit(2);
        }
        buffer[leidos-1] = '\0';
        evento = espera_evento(buffer);
        switch(evento){
            case ev_msj:
                if(estado != BLOQUEO){
                    printf("%s\n", buffer);
                }else{
                    printf("Usted ya no esta autorizado\n");
                }
                break;
            case ev_taco:
                if(estado == EST_OCIOSO){
                    printf("%s\n", buffer);
                }else if(estado == EST_FILTRANDO){
                    i = 0;
                    n = 0;
                    do{
                        if((buffer[i] < 32) || ((buffer[i] > 47) && (buffer[i] < 58)) || (buffer[i] > 63) && (buffer[i] < 240) && (buffer[i] != '\0')){
                            plb[n] = buffer[i];
                        }else{
                            plb[n] = '\0';
                            if(!strcmp("taco", plb)){
                                write(1, "XXXX ", 6);
                                cont_tacos++;
                            }else{
                                write(1, plb, n+1);
                                write(1, " ", 2);
                            }                            
                            n=-1;
                        }
                        i++;
                        n++;
                    }while(i < leidos);

                    plb[n] = '\0';
                    if(!strcmp("taco", plb)){
                        write(1, "XXXX ", 6);
                        cont_tacos++;
                    }else{
                        write(1, plb, n);
                        write(1, " ", 2);
                    }                            
                    write(1, "\n", 2);

                    if(cont_tacos >= 3){
                        cont_tacos = 0;
                        printf("<|Filtrando --> Bloqueo|>\n");
                        estado = BLOQUEO;
                    }
                }else{
                    printf("Usted ya no esta autorizado\n");
                }
                break;
            case ev_on:
                if(estado == EST_OCIOSO){
                    estado = EST_FILTRANDO;
                    printf("<|Ocioso --> Filtrando >\n");
                }
                break;
            case ev_off:
                if(estado != EST_OCIOSO){
                    estado--;
                    printf("Offf\n");
                }
                break;
            case ev_fin:
                fin = true;
                break;
        }
    }
}