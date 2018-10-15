#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include<signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

#define MSG_SIZE 250
#define MAX_CLIENTS 20

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

/*
    En el logIn debo comprobar:
        - Que no este lleno el server
        - Que este registrado/que la contrasena y usuario esten 
            registrados
        - standby hasta que decida buscar partida
*/
void logIn(int &new_sd, int sd, struct from, socklen_t from_len, int &numClientes, int arrayClientes, fd_set &readfds, char buffer);
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
                        
                        logIn();

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
                        
                        recibidos = recv(i,buffer,sizeof(buffer),0);
                        
                        if(recibidos > 0){
                                
                            if(strcmp(buffer,"SALIR\n") == 0)
                                salirCliente(i,&readfds,&numClientes,arrayClientes);        
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
                    }
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

void logIn(int &new_sd, int sd, struct from, socklen_t from_len, int &numClientes, int arrayClientes, fd_set &readfds, char buffer){

    if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
        perror("Error aceptando peticiones");
    else{
        if(numClientes < MAX_CLIENTS){
            arrayClientes[numClientes] = new_sd;
            numClientes++;
            FD_SET(new_sd,&readfds);

            strcpy(buffer, "Para continuar, introduzca usuario y contrasena!\n");
            send(new_sd,buffer,strlen(buffer),0);

            //Espero la llegada de la respuesta del usuario v1 (username)
            strcpy(buffer, "Username: ");
            send(new_sd,buffer,strlen(buffer),0);
            bzero(buffer,sizeof(buffer));            
            if( recv(i,buffer,sizeof(buffer),0) > 0){
                //buffer es ahora mismo el username
                char username;
                strcpy(username, buffer);
            }

            //Espero la llegada de la respuesta del usuario v2 (pass)
            strcpy(buffer, "Password\n");
            send(new_sd,buffer,strlen(buffer),0);
            bzero(buffer,sizeof(buffer));            
            if( recv(i,buffer,sizeof(buffer),0) > 0){
                //buffer es ahora mismo el pass
                char pass;
                strcpy(pass, buffer);
            }

            //Ahora toca comprobar dicho usuario en nuestra base de datos.
            int found, nuevoRegistro;
            bool loginComplete = false;

            do{
                ifstream file;
                file.open("userDatabase.txt");
                if (file.is_open()){
                    do (getline (file, userEntry)){
                        found = strcmp(userEntry, username + ":" + pass);
                    }while(found != 0)
                    file.close();
                }

                if(found)
                    loginComplete = true;
                else{
                    //El usuario no existe o se ha introducido erroneamente
                    strcpy(buffer, "El usuario no existe o se ha introducido erroneamente. Si no posee aun cuenta, introduzca Y para crear una cuenta. Si cree que se ha equivocado, pulse N para cancelar el nuevo registro.\n");
                    send(new_sd,buffer,strlen(buffer),0);
                    //Espero la respuesta
                    bzero(buffer,sizeof(buffer));            
                    if( recv(i,buffer,sizeof(buffer),0) > 0)
                        nuevoRegistro = strcmp(buffer, "N");

                    if(nuevoRegistro == 0){
                        //Esto quiere decir que existe usuario y desea repetir el login
                        loginComplete = false;
                    }else{
                        //Esto quiere decir que quiere realizar el registro con la informacion suministrada.
                        strcpy(buffer, "Estamos registrando su login introducido en nuestro sistema. Bienvenido %s! (su contrasena es %s)\n", username, pass);
                        send(new_sd,buffer,strlen(buffer),0);

                        //Ahora toca guardar la info
                        ofstream file ("userDatabase.txt");
                        if (file.is_open()){
                            file << username << ":" << pass << "\n";
                            file.close();
                        }
                        loginComplete = true;
                    }
                }
            }while(!loginComplete)

            strcpy(buffer, "+0k. Usuario conectado\n");
            send(new_sd,buffer,strlen(buffer),0);

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
