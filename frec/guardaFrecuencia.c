#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 128

int writen(int fd, void * mensaje, int longitud_mensaje);

struct Estructura{
	int frecuencia;
	char palabra[MAX];
};

int main(int argc, void *argv[]){
	if(argc < 2){
		printf("Uso ./gf <...palabras>");
		exit(1);
	}
	/* Abrimos el fichero en modo lectura */
	int fd;
	if((fd = open(argv[1], O_RDONLY)) <  0){
		perror("Fallo en open");
		exit(1);
	}

	struct Estructura lista[MAX];
	for(unsigned k = 0 ; k < MAX ; k++){
		lista[k].frecuencia = -1;
	}
	char buffer[MAX];
	for(unsigned l = 0 ; l < 128 ; l++){
		lista[l].frecuencia = -1;
	}

	/* Lectura de las palabras incluidas como argumento  */
	for(unsigned i = 2 ; i < argc ; i++){
		strcpy(lista[i-2].palabra, argv[i]);
		lista[i-2].frecuencia = 0;
	}

	/* Comprobacion de que se realiza bien la lecura */
	for(unsigned j = 0 ; j < argc-1 ; j++){
		printf("%s\n", lista[j].palabra);
	}

	/* Bucle super optimizado */
	char c;
	bool fin = false;
	int tam = 0;
	int leidos = read(fd,&c,1);

	while(leidos > 0){
		if((int)c < 65 || ((int)c > 90 && (int)c < 97) || (int)c > 122){ /* Compruebo si es una letra */

			buffer[tam]='\0'; /* Fin de string, cuando c es distinto de una letra */
			fin = true; /* Variable de control para comprobar si es una palabra que me interesa */
//			printf("%s\t",buffer);
//			system("read -p 'Press Enter to continue...' var");

		}else{
			buffer[tam] = c; /* Cuando c es una letra lo añado al array de caracteres */
			tam++;
		}

		if(fin == true){
			for(unsigned k = 2; k < argc ; k++){

				if(strcmp(buffer, argv[k]) == 0){
					lista[k-2].frecuencia++;
//					printf("Palabra:\t%s\t||\tFrec:\t%d\n\n", lista[k-2].palabra, lista[k-2].frecuencia);
//					system("read -p 'Press Enter to continue...' var");
				}
			}
			fin = false;
			tam = 0;
		}

		leidos = read(fd,&c,1);
		
	}

	/* Abrimos el fichero binario */
	int bin;
	if((bin = open("./frecuencias.bin", O_WRONLY))< 0){
		perror("Fallo en fichero bin");
		exit(1);	
	}

	printf("\n\n");
	for(unsigned i = 0 ; i < argc-2 ; i++){
		printf("Palabra:\t%s\nFrec:\t%d\n---------------------------\n", lista[i].palabra, lista[i].frecuencia);
		if(writen(bin, &lista[i], sizeof(lista)) < 0){
			perror("Fallo en envio lista.palabra");
			exit(1);
		}
	}



	close(fd);
	close(bin);
	return 0;
}


int writen(int fd, void * mensaje, int longitud_mensaje){
  int a_escribir = longitud_mensaje;
  int total_escrito = 0;
  int escrito;

  do {
    errno = 0;
    escrito = write(fd, mensaje + total_escrito, a_escribir);
    if (escrito >= 0) {
      total_escrito += escrito ;
      a_escribir -= escrito ;
    }
  } while(
        ((escrito > 0) && (total_escrito < longitud_mensaje)) ||
        (errno == EINTR));

  if (total_escrito > 0) {
    return total_escrito;
  } else {
    /* Para permitir que devuelva un posible error de la llamada a write */
    return escrito;
  }
 }
