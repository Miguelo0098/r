#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <ctime>
#include <arpa/inet.h>
#include "tablero.hpp"

#define SERVER_HOST "127.0.0.1"

int main() {
  /* Descriptor del buffer y del socket */

  int sd;
  struct sockaddr_in sockname;
  char buffer[500];
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
  sockname.sin_addr.s_addr = inet_addr("127.0.0.1");

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

   /* Tramitación de la información */

   do {
     auxfds = readfds;
     salida = select(sd+1, &auxfds, NULL, NULL, NULL);

     // Tengo mensaje del servidor
     if (FD_ISSET(sd, &auxfds)) {
       bzero(buffer, sizeof(buffer));
       recv(sd, buffer, sizeof(buffer), 0);

       printf("\n%s\n", buffer);

       if(strcmp(buffer,"Demasiados clientes conectados\n") == 0)
           fin =1;

       if(strcmp(buffer,"Desconexion servidor\n") == 0)
           fin =1;

     }
     else{
       // he introducido mensaje
       if(FD_ISSET(0,&auxfds)){
           bzero(buffer,sizeof(buffer));

           fgets(buffer,sizeof(buffer),stdin);

           if(strcmp(buffer,"SALIR\n") == 0){
                   fin = 1;

           }

           send(sd,buffer,sizeof(buffer),0);

       }
     }
   } while(fin == 0);

  return 0;
}
