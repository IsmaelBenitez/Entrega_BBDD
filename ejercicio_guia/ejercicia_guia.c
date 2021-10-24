#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>

void *AtenderCliente(void *socket)
{
	int sock_conn, ret;
	int *s;
	s=(int *) socket;
	sock_conn=*s;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	//Se acaba de iniiciar
	for(;;){
		
		
		
		int terminar=0;
		
		while (terminar==0){
		//Se recibe la petición
			printf ("Escuchando\n");
			ret=read(sock_conn,peticion, sizeof(peticion));
			printf ("Recibido\n");
			peticion[ret]='\0';
			printf ("Peticion: %s\n",peticion);
			//Finaliza el proceso de recibir la peticion.
			
			//Vamos a ver que quieren
			char *p =strtok(peticion,"/");
			int codigo =atoi(p);
			
			//Ya sabemos que quieren
			float resultado ;
			if (codigo==1){
				p=strtok(NULL,"/");
				float numero=atof (p);
				//En este caso queremos pasar de celsius a farenheit
				resultado =1.8*numero+32;
			}
			else if (codigo==2){
				p=strtok(NULL,"/");
				float numero=atof (p);
				resultado =(numero-32)*5/9;
			}
			else if (codigo==0){
				terminar=1;
			}
			if (codigo!=0){
				sprintf(respuesta,"%f\n",resultado);
				write (sock_conn,respuesta, strlen(respuesta));
			}
			
		}
		// Se acabo el servicio para este cliente
		close(sock_conn); 
		
	}
	
	
}
int main(int argc, char *argv[])
{
	int sock_conn,sock_listen,ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creant socket");
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	serv_adr.sin_port = htons(9000);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	if (listen(sock_listen, 3) < 0)
		printf("Error en el Listen");
	int sockets[100];
	pthread_t thread[100];
	int i=0;
	for (;;)
	{
		printf("Escuchando\n");
		
		sock_conn=accept(sock_listen,NULL,NULL);
		printf("He recibido conexion\n");
		sockets[i]=sock_conn;
		pthread_create(&thread[i],NULL,AtenderCliente,&sockets[i]);
		i++;
	}
	
	
}

