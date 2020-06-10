#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <stdint.h>

#define LISTO 1
#define RETRASO_TX 2
#define ESPERA_CTS 3
#define ERROR 4
#define ESPERA_TX_FINISH 5
#define RETRASO_TX_OFF 6
#define ESPERA_CTS_OFF 7

#define ev_timer 1
#define ev_msj 2
#define ev_cts 3
#define ev_tx_fin 4
#define ev_cts_off 5

#define TAM 128

int fd_pipe[2];
if(pipe(fd_pipe) < 0){
    perror("pipe");
    exit(1);
}

uint8_t espera_evento(){
    uint8_t evento = 0;
    int leidos = read(fd_pipe[0], &evento, 1);
    
    if(leidos < 0){
        perror("read eventos");
        exit(2);
    }
    return eventos;
}

void alarma(int s){
    write(1, "ev_timer\n", strlen("ev_timer\n"));
    int escrito = write(fd_pipe[1], ev_timer, 1);
    if(escrito < 0){
        perror("write ev_timer");
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        exit(3);
    }
}

void manda_msj(int s){
    write(1, "ev_manda_msj\n", strlen("ev_manda_msj\n"));
    int escrito = write(fd_pipe[1], ev_manda_msj, 1);
    if(escrito < 0){
        perror("write ev_manda_msj");
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        exit(4);
    }
}

void cts(int s){
    write(1, "ev_cts\n", strlen("ev_cts\n"));
    int escrito = write(fd_pipe[1], ev_cts, 1);
    if(escrito < 0){
        perror("write ev_cts");
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        exit(5);
    }
}

void tx_finished(int s){
    write(1, "ev_tx_finished\n", strlen("ev_tx_finished\n"));
    int escrito = write(fd_pipe[1], ev_tx_finished, 1);
    if(escrito < 0){
        perror("write ev_cts");
        close(fd_pipe[0]);
        close(fd_pipe[1]);
        exit(5);
    }
}

int main(int argc, void * argv[]){
    uint8_t evento = 0;
    int estado = LISTO;

    signal(SIGALRM, alarma);
    signal(SIGINT, manda_msj);
    signal(SIGTERM, cts);
    signal(SIGUSR1, tx_finished);
    signal(SIGUSR2, cts_off);
    return 0;
}
