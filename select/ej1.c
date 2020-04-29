#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/select.h>
#include <unistd.h>

#define MAX 128

int main(int argc, void *argv[]){

	int fd = open("/tmp/fsc_chat", O_RDONLY) ;
	if(fd< 0){
		perror("Fallo en open fifo");
		exit(1);
	}

	char buffer[MAX]={[0 ... MAX-1] = '\0'};
	char msj[MAX]={[0 ... MAX-1] = '\0'};

	fd_set conjunto, copia;
	FD_ZERO(&conjunto);

	FD_SET(0, &conjunto);
	FD_SET(fd, &conjunto);
	
	int leerfd = 0;
	int leerK = 0, r = -1;
	while(1){
		memcpy(&copia, &conjunto, sizeof(fd_set));

		r = select(fd+1,&copia, NULL, NULL, NULL); 

		if(r < 0){
			perror("Fallo en select");
			exit(1);
		}else if(r == 0){
		       break;	
		}else{
	
			if(FD_ISSET(fd, &copia)){
                leerfd = read(fd, msj, MAX);
                if(leerfd < 0){
                    perror("Fallo en read fifo");
                    exit(1);
                }else if(leerfd == 0){
                    break;
                }else{
                    msj[leerfd] = '\0';
                    printf("fifo: %s", msj);
                }
			}
	
			if(FD_ISSET(0, &copia)){
                leerK = read(0, buffer, MAX);

                if(leerK < 0){
                    perror("Fallo en leer por teclado");
                    exit(1);
                }else if(leerK == 0){
                    break;
                }else{
                    buffer[leerK] = '\0';
                    printf("user: %s", buffer);
                }
			}
		}
	}

	close(fd);
	return 0;
}
