/*--------------------------------------------------------------------------
 * 
 * Servidor simple TCP
 * 
 * Acepta conexiones de clientes y les manda un mensaje de bienvenida
 * 
 * Sintaxis:    servidor_simple_tcp [puerto]
 *              Si puerto no se especifica, se usa el 5193
 *
 *-------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <inttypes.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define PUERTO 5193 /* Puerto por defecto */
#define TAM_BUFFER 512 /* Tamaño del buffer */

int main (int argc, char *argv[]) {
  struct sockaddr_in serv_addr; /* Dirección del servidor (local) */
  struct sockaddr_in cli_addr; /* Dirección del cliente (remoto) */
  int sd, n_sd; /* Descriptores de sockets de conexión y datos */
  uint16_t puerto; /* Numero de puerto */
  socklen_t addr_len; /* Longitud de la dirección */
  int nbytes; /* Número bytes leidos/a escribir */
  char buffer[TAM_BUFFER]; /* Buffer de recepción y envío */

  /* Comprobamos si se ha indicado el puerto como argumento */
  if (argc > 1) {
    puerto = atoi(argv[1]);
    if (puerto <= 0) { /* El puerto ha de ser mayor que 0 */
      printf("ERROR: atoi\n");
      exit(1);
    }
  } else {
    puerto = PUERTO;
  }
  
  /* Rellenamos la dirección del servidor */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(puerto);
  serv_addr.sin_addr.s_addr = INADDR_ANY; /* Escucha por cualquier interfaz */
  
  /* Creamos el socket pasivo (para atender peticiones) */
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    perror("socket");
    exit (1);
  }
  
  /* Vinculamos el socket pasivo a la dirección del servidor */
  if (bind(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind");
    exit(1);
  }

  /* Escuchamos peticiones de clientes en el socket pasivo */
  if (listen(sd, 10) < 0) {
    perror("listen");
    exit (1);
  }

  int leidos = 1;
  /* Bucle del servidor: espera una petición de un cliente y le responde */
  while (1) {
    /* Aceptamos una conexión y creamos un socket para transmisión de datos */
    addr_len = sizeof(cli_addr);
    n_sd = accept(sd, (struct sockaddr *) &cli_addr, &addr_len);
    if (n_sd < 0) {
      perror("accept");
      break; /* Salimos del bucle */
    }
    
    printf("Conexion establecida\n");
    while(leidos > 0){
        /* Respondemos con el mensaje de bienvenida */
        leidos = read(n_sd, buffer, strlen(buffer));
        write(1,buffer, leidos);
    }

    /* Cerramos la conexión con el cliente */
    close(n_sd);
  }
  
  /* Fin del servidor */
  close(sd); /* Cerramos el socket */
  return 0;  
}
