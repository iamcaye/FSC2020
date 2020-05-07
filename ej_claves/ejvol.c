#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TAM 128

ssize_t copiaf(	char * fich_destino,
		char * fich_origen,
		char * usuario,
		char * contrasena);
int buscarUyP(int fd, char * user, char * passwd);

int main(int argc, void * argv[]){
	if(argc < 5){
		printf("Uso: %s <Destino> <Origen> <Usuario> <Contrasenia>\n", argv[0]);
	}

	return 0;
}

ssize_t copiaf(	char * fich_destino, char * fich_origen, char * usuario, char * contrasena){

	int fd_origen(fich_origen, O_RDONLY);
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

	int r = buscarUyP(fd_claves, usuario);
}

int buscarUyP(int fd, char * user, char * passwd){
	char * usuario, contr;
	FILE * file = fopen("claves.txt","r"); //comprobar
	do{
		errores(file!=NULL) char linea[500];
		while(fgets(linea, 500-1, file)!=NULL){
			linea[strlen(linea)-1] = '\0'; 
			printf("%s\n", linea);
		}
		splitUyP(usuario, contr)
	}while(!strcmp(usario, user))
	fclose(file);

	if(!strcmp(usario, user)){
		printf("No se encontro el usuario\n");
		return -2;
	}else if(strcmp(contr, passwd)){
		//return copiarFichero(fd_destino, fd_origen);
		printf("Valido!\n");
		return 0;
	}else{
		printf("Contraseña incorrecta\n");
		return -2;
	}

} 

int splitUyP(char * user, char * passwd, char * linea){
	char [] lineaFichero = linea;
	char * puntero = strstr(lineaFichero, "@");
	*puntero = '\0';
	user = lineaFichero;
	passwd = puntero+1;
	//ahora hay dos cadenas apuntadas por usuario y password
	printf("%s %s\n", user, passwd);
}
