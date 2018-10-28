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
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], Tablero arrayTableros[]);

int traducirCoordenadas(char *c);
bool verificarCoordenadas(int x, int y);

int main () {

    /*----------------------------------------------------
        Descriptor del socket y buffer de datos
    -----------------------------------------------------*/
    int sd, new_sd, salida, arrayClientes[MAX_CLIENTS], numClientes = 0;
    struct sockaddr_in sockname, from;
      Tablero arrayTableros[MAX_CLIENTS/2];
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

                                FD_SET(new_sd,&readfds);

                                strcpy(buffer, "+0k. Usuario conectado.\n");
                                send(new_sd,buffer,strlen(buffer),0);

                                bzero(buffer, sizeof(buffer));
                                //Estamos en espera a recibir un mensaje de algun cliente


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

                            if(strcmp(buffer,"SALIR\n") == 0){
                                salirCliente(i,&readfds,&numClientes,arrayClientes, arrayTableros);
                            }

                            if (strcmp(buffer,"INICIAR-PARTIDA\n") == 0) {
                                bool logged = false;
                                for (int counter = 0; counter < MAX_CLIENTS; counter++) {
                                    if (arrayClientes[counter] == i) {
                                        logged = true;
                                        bzero(buffer,sizeof(buffer));
                                        strcpy(buffer, "+Ok. Buscando hueco en un tablero.\n");
                                        send(i,buffer,strlen(buffer),0);
                                        for (k = 0; k < MAX_CLIENTS/2; (k++)%MAX_CLIENTS/2) {
                                            if (arrayTableros[k].getJugadorA() == 0) {
                                                arrayTableros[k].setJugadorA(i);
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer, "+Ok. Eres el jugador A\n");
                                                send(i,buffer,strlen(buffer),0);

                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer, "+Ok. Esperando un oponente...\n");
                                                send(i,buffer,strlen(buffer),0);
                                                break;
                                            }else if (arrayTableros[k].getJugadorA() == i) {
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer, "-Err. Ya estás en una partida.\n");
                                                send(i,buffer,strlen(buffer),0);
                                                break;
                                            }else if (arrayTableros[k].getJugadorB() == 0) {
                                                arrayTableros[k].setJugadorB(i);
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer, "+Ok. Eres el jugador B\n");
                                                send(i,buffer,strlen(buffer),0);

                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer, arrayTableros[k].printTablero().c_str());
                                                send(arrayTableros[k].getJugadorA(), buffer, strlen(buffer),0);
                                                send(arrayTableros[k].getJugadorB(), buffer, strlen(buffer),0);
                                                break;
                                            }else if (arrayTableros[k].getJugadorB() == i) {
                                                bzero(buffer,sizeof(buffer));
                                                strcpy(buffer, "-Err. Ya estás en una partida.\n");
                                                send(i,buffer,strlen(buffer),0);
                                                break;
                                            }
                                        }
                                    }
                                }
                                if (logged == false) {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer, "-Err. Debes iniciar sesión para iniciar partida.\n");
                                    send(i,buffer,strlen(buffer),0);
                                }
                            }

                            if(strncmp(buffer, "USUARIO ", 8) == 0){
                                //El cliente intenta ingresar su usuario, por lo que verifico si se encuentra en nuestra base de datos.
                                //Para ello, antes separo el "USUARIO " de lo que es el usuario en si.
                                char *aux = strtok(buffer, " ");
                                aux = strtok(NULL, " ");
                                aux[strlen(aux)-1] = '\0';

                                char user[strlen(aux)];
                                strcpy(user, aux);

                                //Ya tenemos la variable user con el nombre de usuario en si. Procedemos a buscarlo en la base de datos.
                                file.open("userDatabase.txt");
                                if(file.is_open()){
                                    //Creo variables aux (en la que se almacenara cada linea leida) y found (como valor de do while para ver si lo ha encontrado y puede parar)
                                    std::string aux2;
                                    foundUsername = 0;

                                    //Va leyendo linea a linea
                                    while(!foundUsername && getline (file, aux2)){
                                        //Si coinciden ambas cadenas, found se vuelve true y termina las iteraciones
                                        if(strncmp(aux2.c_str(), user, strlen(user)-1) == 0)
                                            foundUsername = 1;
                                        else
                                            foundUsername = 0;
                                    }
                                    file.close();
                                }//End file open

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
                                            char *aux = strtok(buffer, " ");
                                            aux = strtok(NULL, " ");
                                            aux[strlen(aux)-1] = '\0';

                                            char pass[strlen(aux)];
                                            strcpy(pass, aux);

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

                                                while(!successfulLogIn && getline(file, aux)){
                                                    //Si coinciden ambas cadenas, found se vuelve true y termina las iteraciones
                                                    if(strncmp(aux.c_str(), userCredentials, strlen(userCredentials)) == 0 )
                                                        successfulLogIn = true;
                                                }

                                                file.close();
                                            }//End file open

                                            if(successfulLogIn){
                                                //Usuario y contrasena coinciden. Usuario conectado.
                                                strcpy(buffer, "+0k. Usuario validado.");
                                                send(new_sd,buffer,strlen(buffer),0);

                                                arrayClientes[numClientes] = new_sd;
                                                numClientes++;
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
                                            }//End of log ins

                                        }else{ //End of the password obtain
                                            strcpy(buffer, "-Err. Error en la validacion.");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }//Espero contrasena pero no FIN
                                    }//recv end

                                }else{//foundUsername is false
                                    //En este condicional entra cuando el usuario introducido previamente no esta en el fichero. Se repite desde cero el logIn.
                                    strcpy(buffer, "-Err. Usuario incorrecto.");
                                    send(new_sd,buffer,strlen(buffer),0);
                                }
                            }//FIN LOGIN NORMAL

                            if(strncmp(buffer, "REGISTRO ", 9) == 0){
                                //Saco el usuario del buffer
                                char *aux = strtok(buffer, " ");

                                //Una vez tenemos en aux el buffer, vamos avanzando en la frase
                                //Este strtok mete en aux "-u"
                                aux = strtok(NULL, " ");

                                //Este strtok mete en aux el usuario en si (ESTE ES UTIL)
                                aux = strtok(NULL, " ");
                                char user[strlen(aux)];
                                strcpy(user, aux);

                                //Este strtok mete en aux "-p"
                                aux = strtok(NULL, " ");

                                //Este strtok mete en aux la password (ESTE TAMBIEN ES UTIL)
                                aux = strtok(NULL, " ");
                                aux[strlen(aux)-1] = '\0';
                                char pass[strlen(aux)];
                                strcpy(pass, aux);

                                //Ya tenemos las variables para crear el registro. Abrimos el archivo que contiene los usuarios y respectivas passwords.
                                file.open("userDatabase.txt");
                                if(file.is_open()){
                                    std::string aux;
                                    bool foundExistingUser = false;
                                    char userCredentials[strlen(user) + strlen(pass) + 1];
                                    //Creo la combinacion de "usuario:password"
                                    strcpy(userCredentials, user);
                                    strcat(userCredentials, ":");
                                    strcat(userCredentials, pass);

                                    while(!foundExistingUser && getline(file, aux)){
                                        //Si coinciden ambas cadenas, found se vuelve true y termina las iteraciones
                                        if(strncmp(aux.c_str(), userCredentials, strlen(userCredentials)) == 0 )
                                            foundExistingUser = true;
                                    }

                                    //Cierro el fichero en modo lectura
                                    file.close();

                                    //Paso a string el char que contiene las credenciales
                                    std::string userCreds(userCredentials);

                                    //Ahora abro el fichero en modo escritura (append)
                                    if(!foundExistingUser){
                                        //Escribo usuario en fichero
                                        std::ofstream fileRegister;
                                        fileRegister.open("userDatabase.txt", std::ios::app);

                                        if(fileRegister.is_open()){
                                            fileRegister << userCreds << std::endl;

                                            strcpy(buffer, "+0k. Usuario registrado.");
                                            send(new_sd,buffer,strlen(buffer),0);
                                        }

                                    }else{
                                        strcpy(buffer, "-Err. Usuario ya se encuentra registrado.");
                                        send(new_sd,buffer,strlen(buffer),0);
                                    }
                                }

                            }//End register
                        }

                        //Si el cliente introdujo ctrl+c
                        if(!recibidos){
                            printf("El socket %d, ha introducido ctrl+c\n", i);
                            //Eliminar ese socket
                            salirCliente(i,&readfds,&numClientes,arrayClientes, arrayTableros);
                        }

                    }//Fin del else
                }
            }
        }
    }
    close(sd);
    return 0;
}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[], Tablero arrayTableros[]){

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

    // Si estaba en partida, lo Eliminar
    for ( j = 0; j < MAX_CLIENTS/2; j++) {
        if (arrayTableros[j].getJugadorA() == socket) {
            arrayTableros[j].setJugadorA(0);
        }
        else if (arrayTableros[j].getJugadorB() == socket){
            arrayTableros[j].setJugadorB(0);
        }
    }

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

int traducirCoordenadas(char *c){
    return atoi(c - 17);
}

bool verificarCoordenadas(int x, int y){
    if(x >= 0 && x < 10 && y >= 0 && y < 10)
        return true;
    else
        return false;
}
