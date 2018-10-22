#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "tablero.hpp"

#define SERVER_HOST "localhost"

int main() {
  /* Descriptor del buffer y del socket */

  int sd;
  struct sockaddr_in sockname;
  char buffer[500]
  socklen_t len_sockname;
    fd_set readfds, auxfds;
    int salida;
    int fin = 0;

  /* Se abre el socket */

  sd = socket (AF_INET, SOCK_STREAM, 0);
  if (sd == -1) {
    perror("No se puede abrir el socket cliente\n");
    exit(1);
  }

  /* Rellenamos campos de estructura con ip del servidor y el puerto del servicio que solicitamos */

  sockname.sin_family = AF_INET;
  sockname.sin_port = htons(2000);
  sockname.sin_addr.s_addr = inet_addr(SERVER_HOST);

  /* Se solicita la conexión con el servidor */

  len_sockname = sizeof(sockname);

  if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1) {
    perror ("Error de conexion");
    exit(1);
  }

  /* Inicializamos las estructuras */
   FD_ZERO(&auxfds);
   FD_ZERO(&readfds);

   FD_SET(0, &readfds);
   FD_SET(sd, &readfds);


  return 0;
}
