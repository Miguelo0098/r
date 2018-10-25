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
#include <iostream>
#include <fstream>


#define MSG_SIZE 500
#define MAX_CLIENTS 20

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

/*
    En startGame debo:
        - Comprobar que exista otro jugador dispuesto a jugar
        - Establecer el estado de ambos jugadores como "InGame" para
            que no esten en matchmaking
        - Crear el tablero
        - Enviar un tablero sin ningun desbloqueado a ambos jugadores
        - Comenzar a recibir ordenes
*/
void startGame();

/*
    En teoria, aqui se introduce el mensaje recibido del cliente
        y se hacen efectivos los cambios en el tablero. Como respuesta
        esta funcion envia de vuelta al cliente un tablero renovado
        con los cambios realizados.
*/
void makeMove();


int main () {

    /*----------------------------------------------------
        Descriptor del socket y buffer de datos
    -----------------------------------------------------*/
    int sd, new_sd, salida, arrayClientes[MAX_CLIENTS], numClientes = 0;
    struct sockaddr_in sockname, from;
      std::vector<Tablero> arrayTableros;
      arrayTableros.resize(MAX_CLIENTS/2);
      std::vector<int> arrayJugadores;
      arrayJugadores.resize(MAX_CLIENTS);
    char buffer[MSG_SIZE];
    socklen_t from_len;
    fd_set readfds, auxfds;

    //Contadores
    int i,j,k, recibidos;
    char identificador[MSG_SIZE];

    int on, ret;

    /* --------------------------------------------------
        Se abre el socket
    ---------------------------------------------------*/
    sd = socket (AF_INET, SOCK_STREAM, 0);
    if (sd == -1) {
        perror("No se puede abrir el socket cliente\n");
        exit (1);
    }

    // Activaremos una propiedad del socket que permitir· que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permitir· en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
    // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP)

    on = 1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockname.sin_family = AF_INET;
    sockname.sin_port = htons(2000);
    sockname.sin_addr.s_addr =  INADDR_ANY;

    if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1){
        perror("Error en la operación bind");
        exit(1);
    }

    /*---------------------------------------------------------------------
        Del las peticiones que vamos a aceptar sólo necesitamos el
        tamaño de su estructura, el resto de información (familia, puerto,
        ip), nos la proporcionará el método que recibe las peticiones.
    ----------------------------------------------------------------------*/
    from_len = sizeof (from);

    if(listen(sd,1) == -1){
        perror("Error en la operación de listen");
        exit(1);
    }

    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);

    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);

    /*-----------------------------------------------------------------------
        El servidor acepta una petición
    ------------------------------------------------------------------------ */
    while(true){
        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
        auxfds = readfds;
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);

        if(salida > 0){
            for(i = 0; i<FD_SETSIZE; i++){
                //Buscamos el socket por el que se ha establecido la comunicación
                if(FD_ISSET(i, &auxfds)) {
                    if( i == sd){   //En este if se gestiona la entrada de nuevos clientes al servidor

                        //START LOGIN

                        if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                            perror("Error aceptando peticiones");
                        else{
                            if(numClientes < MAX_CLIENTS){
                                arrayClientes[numClientes] = new_sd;
                                numClientes++;
                                FD_SET(new_sd,&readfds);

                                strcpy(buffer, "+0k. Usuario conectado.\n");
                                send(new_sd,buffer,strlen(buffer),0);



                                bzero(buffer, sizeof(buffer));
                                //Estamos en espera a recibir un mensaje de algun cliente
                                if( recv(i, buffer, sizeof(buffer), 0) > 0 ){
                                    printf("Estoy vivo\n");
                                    //Comprobamos el tipo de mensaje que acabamos de recibir por un cliente

                                }

                            }else{
                                bzero(buffer,sizeof(buffer));
                                strcpy(buffer,"Demasiados clientes conectados\n");
                                send(new_sd,buffer,strlen(buffer),0);
                                close(new_sd);
                            }
                        }

                        //ENDLOGIN

                    } //Fin entrada nuevos usuarios
                     else if (!i){
                        //Se ha introducido información de teclado en el proceso de servidor
                        bzero(buffer, sizeof(buffer));
                        fgets(buffer, sizeof(buffer), stdin);

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
                        //Si el mensaje no da lugar a que se cierre el servidor se hace lo siguiente!
                        //Mensajes que se quieran mandar a los clientes (implementar)
                    }else{ // Si i no es nula
                        //Aqui analizo lo que el usuario me ha enviado
                        bzero(buffer,sizeof(buffer));
                        std::ifstream file;
                        bool foundUsername;
                        bool successfulLogIn;
                        recibidos = recv(i,buffer,sizeof(buffer),0);

                        if(recibidos > 0){

                            if(strcmp(buffer,"SALIR\n") == 0)
                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                            if(strncmp(buffer, "USUARIO ", 8) == 0){
                                //El cliente intenta ingresar su usuario, por lo que verifico si se encuentra en nuestra base de datos.
                                //Para ello, antes separo el "USUARIO " de lo que es el usuario en si.
                                char *user = strtok(buffer, " ");
                                user = strtok(NULL, " ");

                                //Ya tenemos la variable user con el nombre de usuario en si. Procedemos a buscarlo en la base de datos.
                                file.open("userDatabase.txt");
                                if(file.is_open()){
                                    //Creo variables aux (en la que se almacenara cada linea leida) y found (como valor de do while para ver si lo ha encontrado y puede parar)
                                    std::string aux;
                                    aux.resize(strlen(user));
                                    foundUsername = false;


                                        //Va leyendo linea a linea
                                    while(!foundUsername && getline (file, aux)){
                                        //Si coinciden ambas cadenas, found se vuelve true y termina las iteraciones
                                        if(strncmp(aux.c_str(), user, strlen(user)) == 0)
                                            foundUsername == true;
                                            printf("HOLA estoy vivo salvame\n");
                                    }
                                    file.close();
                                }

                                if(foundUsername){
                                    //En este condicional entra cuando el usuario introducido previamente esta en el fichero.
                                    strcpy(buffer, "+Ok. Usuario correcto.");
                                    send(new_sd,buffer,strlen(buffer),0);

                                    //Espero la llegada de PASSWORD password
                                    bzero(buffer,sizeof(buffer));
                                    if( recv(i,buffer,sizeof(buffer),0) > 0){
                                        //Comprobamos el tipo de mensaje que acabamos de recibir por un cliente
                                        if(strncmp(buffer, "PASSWORD ", 9) == 0){
                                            //El cliente intenta ingresar su password, por lo que verifico si se encuentra en nuestra base de datos.
                                            //Para ello, antes separo el "PASSWORD " de lo que es el usuario en si.
                                            char *pass = strtok(buffer, " ");
                                            pass = strtok(NULL, " ");
                                            pass[strlen(pass)-1] = '\0';

                                            //Ya tenemos pass preparada para comprobar
                                            file.open("userDatabase.txt");
                                            if(file.is_open()){
                                                std::string aux;
                                                aux.resize(strlen(pass));
                                                successfulLogIn = false;
                                                char userCredentials[strlen(user) + strlen(pass) + 1];
                                                //Creo la combinacion de "usuario:password"
                                                strcpy(userCredentials, user);
                                                strcat(userCredentials, ":");
                                                strcat(userCredentials, pass);

                                                do{
                                                    getline(file, aux);
                                                    if(strncmp(aux.c_str(), userCredentials, strlen(userCredentials)) == 0 )
                                                        successfulLogIn = true;

                                                }while(!successfulLogIn);
                                                file.close();
                                            }

                                            if(successfulLogIn){
                                                //Usuario y contrasena coinciden. Usuario conectado.
                                                strcpy(buffer, "+0k. Usuario validado.");
                                                send(new_sd,buffer,strlen(buffer),0);

                                                //Uwu
                                                //Esto siguiente es por si ademas del login, quisieramos avisar al resto de usuarios. (NO ES NECESARIO)
                                                for(j = 0; j < (numClientes-1); j++){
                                                    bzero(buffer,sizeof(buffer));
                                                    sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
                                                    send(arrayClientes[j],buffer,strlen(buffer),0);
                                                }

                                            }else{
                                                //La contrasena es incorrecta
                                                strcpy(buffer, "-Err. Error en la validacion.");
                                                send(new_sd,buffer,strlen(buffer),0);
                                            }
                                        }
                                    }

                                    }else{
                                        //En este condicional entra cuando el usuario introducido previamente no esta en el fichero. Se repite desde cero el logIn.
                                        strcpy(buffer, "-Err. Usuario incorrecto.");
                                        send(new_sd,buffer,strlen(buffer),0);
                                    }
                                }
                            else{
                                sprintf(identificador,"%d: %s",i,buffer);
                                bzero(buffer,sizeof(buffer));
                                strcpy(buffer,identificador);

                                for(j = 0; j < numClientes; j++)
                                    if(arrayClientes[j] != i)
                                        send(arrayClientes[j],buffer,strlen(buffer),0);
                            }
                        }
                        //Si el cliente introdujo ctrl+c
                        if(!recibidos){
                            printf("El socket %d, ha introducido ctrl+c\n", i);
                            //Eliminar ese socket
                            salirCliente(i,&readfds,&numClientes,arrayClientes);
                        }

                    }//Fin del else
                }
            }
        }
    }
    close(sd);
    return 0;

}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){

    char buffer[250];
    int j;

    close(socket);
    FD_CLR(socket,readfds);

    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);

    (*numClientes)--;

    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente: %d\n",socket);

    for(j = 0; j < (*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,strlen(buffer),0);
}

void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);

    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
    exit(0);
}
/* Comentada funcion logIn

void logIn(int &new_sd, int sd, struct from, socklen_t from_len, int &numClientes, int arrayClientes, fd_set &readfds, char* buffer){

    if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
        perror("Error aceptando peticiones");
    else{
        if(numClientes < MAX_CLIENTS){
            arrayClientes[numClientes] = new_sd;
            numClientes++;
            FD_SET(new_sd,&readfds);

            strcpy(buffer, "+0k. Usuario conectado.\n");
            send(new_sd,buffer,strlen(buffer),0);

            std::ifstream file;
            file.open("userDatabase.txt");

            bzero(buffer, sizeof(buffer));
            //Estamos en espera a recibir un mensaje de algun cliente
            if( recv(i, buffer, sizeof(buffer, 0)) > 0 ){
                //Comprobamos el tipo de mensaje que acabamos de recibir por un cliente
                if(strncomp(buffer, "USUARIO ", 8) == 0){
                    //El cliente intenta ingresar su usuario, por lo que verifico si se encuentra en nuestra base de datos.
                    //Para ello, antes separo el "USUARIO " de lo que es el usuario en si.
                    char *user = strtok(buffer, "");
                    user = strtok(NULL, "");
                    user[strlen(user)-1] = '\0';
                    //Ya tenemos la variable user con el nombre de usuario en si. Procedemos a buscarlo en la base de datos.
                    if(file.is_open()){
                        //Creo variables aux (en la que se almacenara cada linea leida) y found (como valor de do while para ver si lo ha encontrado y puede parar)
                        char *aux[strlen(user)];
                        bool found = false;
                        //Va leyendo linea a linea
                        do (getline (file, aux)){
                            //Si coinciden ambas cadenas, found se vuelve true y termina las iteraciones
                            if(strcmp(aux, user, strlen(user)) == 0)
                                found == true;

                        }while(!found);
                        file.close();
                    }

                    if(found){
                        strcpy(buffer, "+Ok. Usuario correcto.");
                        send(new_sd,buffer,strlen(buffer),0);

                        //Espero la llegada de PASSWORD password
                        bzero(buffer,sizeof(buffer));
                        if( recv(i,buffer,sizeof(buffer),0) > 0){

                        }

                    }else{
                        strcpy(buffer, "-Err. Usuario incorrecto.");
                        send(new_sd,buffer,strlen(buffer),0);
                    }

                }
            }

            for(j = 0; j < (numClientes-1); j++){
                bzero(buffer,sizeof(buffer));
                sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
                send(arrayClientes[j],buffer,strlen(buffer),0);
            }

        }else{
            bzero(buffer,sizeof(buffer));
            strcpy(buffer,"Demasiados clientes conectados\n");
            send(new_sd,buffer,strlen(buffer),0);
            close(new_sd);
        }
    }
}

Comentada funcion login */
