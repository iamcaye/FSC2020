#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TAM 128


ssize_t copiaf(	char * fich_destino,
		char * fich_origen,
		char * usuario,
		char * contrasena);
int buscarUyP(int fd, char * user, char * passwd);

int copiar(int origen, int destino);

int main(int argc, void * argv[]){
	if(argc < 5){
		printf("Uso: %s <Destino> <Origen> <Usuario> <Contrasenia>\n", argv[0]);
        exit(1);
	}

    int res = copiaf(argv[1], argv[2], argv[3], argv[4]);
    if(res < 0){
        printf("Ocurrio un error...\n");
    }else{
        printf("%d bits copiados con exito!\n", res);
    }

	return 0;
}

void splitUyP(char * user, char * passwd, char * linea);

ssize_t copiaf(	char * fich_destino, char * fich_origen, char * usuario, char * contrasena){

	int fd_origen = open(fich_origen, O_RDONLY);
	if(fd_origen < 0){
		perror("El fichero no existe");
		return -1;
	}
	int fd_destino = creat(fich_destino, 0666);
	if(fd_destino < 0){
		perror("fallo en open fichero destino");
		return -1;
	}

	int fd_claves = open("claves.txt", O_RDONLY);
	if(fd_claves < 0){
		perror("Fallo en abrir fichero de claves");
		return -1;
	}

	int r = buscarUyP(fd_claves, usuario, contrasena);

    if(r > 0){
        copiar(fd_origen, fd_destino);
    }else{
        return -2;
    }
}

int buscarUyP(int fd, char * user, char * passwd){
	char * usuario;
    char * contr;
    const char * msj;
    printf("Buscando usuario y contraseña (%s)\n", user);

    int leidos = 0, total = 0;
    do{
        leidos = 0; total = 0;
        char c;
        char buffer[TAM];
        do{
            leidos = read(fd,&c,1);
            buffer[total] = c;
            total += leidos;
        }while((leidos > 0) && (c != '\n'));
        buffer[total-1]= '\0';

        if(leidos > 0){
        	char * puntero = strstr(buffer, "@");
        	*puntero = '\0';
        	usuario = buffer;
        	contr = puntero+1;
        	//ahora hay dos cadenas apuntadas por usuario y password
        }else{
            break;
        }

    }while(strcmp(user,usuario) != 0);
    
    if(strcmp(user,usuario) != 0){
        printf("Usuario no encontrado!\n");
        return -2;
    }else if(strcmp(passwd,contr) != 0){
        printf("Contraseña incorrecta!\n");
        return -2;
    }else{
        printf("Usuario identificado\n");
        return 1;
    }
} 


int copiar(int origen, int destino){
    char copia[TAM];
    int leidos = 0;
    int total = 0;
    int escrito = 0;

    do{
        leidos = read(origen, copia, TAM);
        total += leidos;
        escrito = write(destino, copia, leidos);
    }while((leidos > 0) && (escrito==leidos));

    if((leidos < 0) || (escrito != leidos)){
        printf("Fallo en lectura/escritura\n");
        return -1;
    }else{
        return total;
    }
}
