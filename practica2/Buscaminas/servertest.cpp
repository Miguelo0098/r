#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include "tablero.hpp"
#include <vector>


#define MSG_SIZE 500
#define MAX_CLIENTS 20

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

int main(int argc, char const *argv[]) {
  int sd, new_sd;
  int salida;
  int arrayClientes[MAX_CLIENTS];
  int numClientes = 0;
  struct sockaddr_in sockname, from;

  Tablero arrayTableros[MAX_CLIENTS/2];
  std::vector<int> arrayJugadores;
  arrayJugadores.resize(MAX_CLIENTS);

  char buffer[MSG_SIZE];
  socklen_t from_len;
  fd_set readfds, auxfds;


  int recibidos;
  char identificador[MSG_SIZE];

  int on, ret;

  /* ABRIR SOCKET */

  sd = socket (AF_INET, SOCK_STREAM, 0);
  if (sd == -1) {
    perror("No se pudo abrir el socket cliente\n");
    exit(1);
  }

  /* Reutilizacion de cualquier puerto al que enlacemos */

  on = 1;
  ret = setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

  sockname.sin_family = AF_INET;
  sockname.sin_port = htons(2000);
  sockname.sin_addr.s_addr = INADDR_ANY;

  if (bind(sd, (struct sockaddr *) &sockname, sizeof(sockname)) == -1) {
    perror("Error en la operación bind");
    exit(1);
  }

  /* De las peticiones que aceptamos solo necesitamos el tamaño de su estructura */

  from_len = sizeof(from);

  if (listen(sd, 1) == -1) {
    perror("Error en la operación de listen");
    exit(1);
  }

  /* Inicializamos conjuntos fd_set */
  FD_ZERO(&readfds);
  FD_ZERO(&auxfds);
  FD_SET(sd, &readfds);
  FD_SET(0, &auxfds);

  /* Capturamos señal SIGINT */
  signal(SIGINT, manejador);

  /* El servidor acepta una petición */

  while (1) {
    /* Esperamos recibir mensajes (nuevas conexiones) */
    auxfds = readfds;
    salida = select(FD_SETSIZE, &auxfds, NULL, NULL, NULL);
    if (salida > 0) {
      for (int i = 0; i < FD_SETSIZE; i++) {

        /* Buscamos el socket por el que se ha establecido la conexion */
        if (FD_ISSET(i, &auxfds)) {
          if (i == sd) {
            if ((new_sd = accept(sd, (struct sockaddr *) &from, &from_len)) == -1) {
              perror("Error aceptando peticiones");
            }
            else{
              if (numCLientes < MAX_CLIENTS) {
                arrayClientes[numClientes] = new_sd;
                numClientes++;
                FD_SET(new_sd, &readfds);

                strcpy(buffer, "Bienvenido al BUSCAMINAS ONLINE\nPor favor, inicia sesión o registrate para jugar");
                send(new_sd, buffer, strlen(buffer), 0);
              }
              else{
                bzero(buffer, sizeof(buffer));
                strcpy(buffer, "Demasiados clientes conectados\n");
                send(new_sd, buffer, strlen(buffer),0);
                close(new_sd);
              }
            }
          }
          else if (i == 0){
            //Se ha introducido información de teclado
            bzero(buffer, sizeof(buffer));
            fgets(buffer, sizeof(buffer),stdin);

            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
            if(strcmp(buffer,"SALIR\n") == 0){

              for (j = 0; j < numClientes; j++){
                send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                close(arrayClientes[j]);
                FD_CLR(arrayClientes[j],&readfds);
              }
              close(sd);
              exit(-1);
            }
          }
        }
      }
    }
  }




  return 0;
}
