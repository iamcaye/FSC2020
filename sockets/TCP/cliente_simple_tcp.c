/*----------------------------------------------------------------------------
 *
 * Cliente simple TCP
 *
 * Se conecta a un servidor que le envía un mensaje y lo muestra por pantalla
 *
 * Sintaxis:  cliente_simple_tcp [direccionIP] [puerto]
 * NOTA:      Si direccionIP no se especifica, se usa localhost
 *            Si puerto no se especifica, se usa el 5193
 *--------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define TAM_BUFFER 512 /* Tamaño de buffer de envío y recepción */
#define DIRECCION_IP "127.0.0.1" /* Dirección IP del servidor por defecto */
#define PUERTO 5193 /* Puerto del servidor por defecto */

int main(int argc, char **argv) {

  struct sockaddr_in serv_addr; /* Dirección del servidor */
  int sd; /* Socket de datos */
  uint32_t direccion_IP; /* Dirección IP del servidor */
  uint16_t puerto; /* Puerto del servidor */
  char buffer[TAM_BUFFER]; /* Buffer de envío */
  int nbytes; /* Número bytes leidos/a escribir */
  
  /* Comprobamos los parámetros de entrada */
  if (argc >= 2) {
    direccion_IP = inet_addr(argv[1]);
    if (direccion_IP < 0) {
      printf("ERROR: inet_addr\n");
      exit(1);
    }
  } else {
    direccion_IP = inet_addr(DIRECCION_IP); /* Dirección IP por defecto */
  }
  
  if (argc == 3) {
    puerto = atoi(argv[2]);
    if (puerto <= 0) { /* El puerto ha de ser mayor que 0 */
      printf("ERROR: atoi\n");
      exit(1);
    }
  } else {
    puerto = PUERTO; /* Puerto por defecto */
  }
  
  /* Rellenamos la dirección del servidor */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(puerto);
  memcpy(&serv_addr.sin_addr, &direccion_IP, 4);
  
  /* Creamos el socket de datos */
  sd = socket(PF_INET, SOCK_STREAM, 0);
  if (sd < 0) {
    perror("socket");
    exit(1);
  }
  
  /* Establecemos la conexión con el servidor */
  if (connect(sd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    perror("connect");
    exit(1);
  }
  
  /* Bucle del cliente:
     recibe datos del servidor hasta que el servidor cierre la conexión */
  while ((nbytes = read(sd, buffer, TAM_BUFFER)) > 0) {
    /* Escribimos los datos recibidos por pantalla */
    write(1, buffer, nbytes);
  }
  
  /* Cerramos el socket */
  close(sd);
  
  printf("\nFin!\n");
  
  return 0;
}
