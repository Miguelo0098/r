#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <locale.h>


/*
 * El servidor ofrece el servicio de incrementar un n\ufffdmero recibido de un cliente
 */
 
int main ()
{
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int Socket_Servidor;
	struct sockaddr_in Servidor;
	char Dato[80];
	
	
	/* -----------------------------------------------------
   		Informaci\ufffdn del Cliente
   	-----------------------------------------------------*/
   struct sockaddr_in Cliente;
	socklen_t Longitud_Cliente;
	
	
	/* --------------------------------------------------
		Se abre el socket Servidor
	---------------------------------------------------*/
	Socket_Servidor = socket (AF_INET, SOCK_DGRAM, 0);
	if (Socket_Servidor == -1)
	{
		printf ("No se puede abrir socket servidor\n");
		exit (-1);	
	}

	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura servidor, necesaria
		para la llamada a la funcion bind().
	-------------------------------------------------------------------*/
	Servidor.sin_family = AF_INET;
	Servidor.sin_port = htons(2000);
	Servidor.sin_addr.s_addr = htonl(INADDR_ANY); 

	if (bind (Socket_Servidor, (struct sockaddr *)&Servidor, sizeof (Servidor)) == -1)
	{
		close (Socket_Servidor);
		exit (-1);
	}
	
	time_t tiempo;
	struct tm * stTm;
	
	tiempo = time(NULL);
	
	stTm = localtime(&tiempo);
	setlocale(LC_ALL, "");

	strftime(Dato,80,"%A, %d de %B", stTm);

  	/*---------------------------------------------------------------------
		Del cliente s\ufffdlo necesitamos el tama\ufffdo de su estructura, el 
		resto de informaci\ufffdn (familia, puerto, ip), la obtendremos 
		nos la proporcionar\ufffd el propio m\ufffdtodo recvfrom, cuando
		recibamos la llamada de un cliente.
   ----------------------------------------------------------------------*/
	Longitud_Cliente = sizeof (Cliente);

   	
	/*-----------------------------------------------------------------------
		El servidor espera continuamente los mensajes de los clientes
	------------------------------------------------------------------------ */
	while (1)
	{
    /* -----------------------------------------------------------------
			Esperamos la llamada de alg\ufffdn cliente
		-------------------------------------------------------------------*/
      	int recibido = recvfrom (Socket_Servidor, &Dato, sizeof(Dato), 0,
			(struct sockaddr *) &Cliente, &Longitud_Cliente);

		/* -----------------------------------------------------------------
			Comprobamos si hemos recibido alguna informaci\ufffdn 
		-------------------------------------------------------------------*/
		if (recibido > 0)
		{
			/*-----------------------------------------------------------------
				Imprimimos el valor que nos ha enviado el cliente 
				------------------------------------------------------------------*/
      	printf ("Recibido %s\n", Dato);

      	if (strcmp(Dato, "DAY\n") == 0)
      	{
      		strftime(Dato,80,"%A, %d de %B de %Y", stTm);
      	}
      	if (strcmp(Dato, "TIME\n") == 0)
      	{
      		strftime(Dato,80,"%H:%M:%S", stTm);
      	}
      	if (strcmp(Dato, "DAYTIME\n") == 0)
      	{
      		strftime(Dato,80,"%A, %d de %B de %Y; %H:%M:%S", stTm);
      	}
      
      	/* ------------------------------------------------------------------
				Devolvemos el n\ufffdmero incrementado al cliente
				--------------------------------------------------------------------*/
			int enviado = sendto (Socket_Servidor, &Dato, sizeof(Dato), 0,
			(struct sockaddr *) &Cliente, Longitud_Cliente);

		}

    }
	 
	 close(Socket_Servidor);

	return 0;
}
