#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <string.h>

/* --------------------------------------------------------------------------------------
 
 Envía un número aleatorio al servidor, quien el devuelve el número incremetnado

---------------------------------------------------------------------------------------- */
 
int main ( )
{
  
		/*---------------------------------------------------- 
			Descriptor del socket y buffer para datos 
		-----------------------------------------------------*/
		int Socket_Cliente;
		char Datos[80];
   
   	/* -----------------------------------------------------
   		Informaci\ufffdn del Servidor
   	-----------------------------------------------------*/
   	struct sockaddr_in Servidor;  
   	socklen_t Longitud_Servidor;
  
  
   	/* --------------------------------------------------
			Se abre el socket cliente
		---------------------------------------------------*/
		Socket_Cliente = socket (AF_INET, SOCK_DGRAM, 0);
		if (Socket_Cliente == -1)
		{
			printf ("No se puede abrir el socket cliente\n");
    		exit (-1);	
		}

 		/*---------------------------------------------------------------------
			Necesitamos una estructura con la informaci\ufffdn del Servidor
			para poder solicitarle un servicio.
   		----------------------------------------------------------------------*/
		Servidor.sin_family = AF_INET;
		Servidor.sin_port = htons(2000);
		Servidor.sin_addr.s_addr =  inet_addr("127.0.0.1");
  	 	Longitud_Servidor = sizeof(Servidor);
	

		/*------------------------------------------------------------------------
			Se genera un n\ufffdmero aleatorio, que es el que se le manda al
			servidor.
		------------------------------------------------------------------------ */

  	printf("Introduzca lo que desea recibir:\n");
	fgets(Datos, sizeof(Datos), stdin);	
   	printf ("Envio %s\n", Datos);

   	/////////////////TEST ZONE///////////////////////
	int counter = 0;

	while(counter < 3){
		
	}




   	///////////////////////////////////////////

		/*-----------------------------------------------------------------------
			Se env\ufffda mensaje al Servidor
		-----------------------------------------------------------------------*/
		int enviado = sendto (Socket_Cliente, (char *) &Datos, sizeof(Datos), 0,
			(struct sockaddr *) &Servidor, Longitud_Servidor);
   
   	if (enviado < 0)
    	{
			printf("Error al solicitar el servicio\n");
		}
    	else
		{
   
		/*----------------------------------------------------------------------
			Esperamos la respuesta del Servidor
			----------------------------------------------------------------------- */ 
			int recibido = recvfrom (Socket_Cliente, (char *)&Datos, sizeof(Datos), 0,
			(struct sockaddr *) &Servidor, &Longitud_Servidor);
			
   		if (recibido > 0)
      		printf ("Leido %s\n", Datos);
   		else
      		printf ("Error al leer del servidor\n");
		
		}
		
		close(Socket_Cliente);

	return 0;
}
