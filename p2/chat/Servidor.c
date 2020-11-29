#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
/*Servidor*/

int main()
{
time_t horario;

int udp_socket,lbind,env,rec,lrecv;
struct sockaddr_in SERVIDOR, CLIENTE;

unsigned char buffer[512];
unsigned char paqRec[512];
unsigned char temp[512];
udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

if(udp_socket==-1)
{
    perror("\nError al abrir el socket");
    exit(0);
}
else
{
    perror("\nExito al abrir el socket");
    SERVIDOR.sin_family=AF_INET; /* address family: AF_INET */
    SERVIDOR.sin_port=htons(8080);   /* port in network byte order */
    SERVIDOR.sin_addr.s_addr=INADDR_ANY;
    lbind=bind(udp_socket, (struct sockaddr *)&SERVIDOR,sizeof(SERVIDOR));
    if(lbind==-1)
    {
        perror("\n ERROR EN bind");
        exit(0);
    }
    else 
    {
        perror("\n Exito en el bind");
        /*CLIENTE.sin_family=AF_INET; /* address family: AF_INET */
        /*CLIENTE.sin_port=htons(53);   /* port in network byte order */
       /*CLIENTE.sin_addr.s_addr=inet_addr("8.8.8.8");*/
       printf("\n\n==========INICIAR CHAT==============");
        lrecv=sizeof(CLIENTE);
        // Inicio de bucle recibo el mensaje 
        //
        while(!strstr(buffer, "-fin") || !strstr(paqRec, "-fin")){
        
        	time(&horario);
        	struct tm *tiempo = localtime(&horario);
        	
		rec=recvfrom(udp_socket,paqRec, 512, 0, (struct sockaddr *)&CLIENTE,&lrecv);
		
		//COMPROBACION DE RECIBO
		if (rec==-1)
		{
		    perror("\n Error al Recibir");
		    exit(0);
		}
		else
		{
			//Exito al Recibir el mensaje, listo para contestar
			printf("\n[%d:%d]",tiempo->tm_hour, tiempo->tm_min);
		    	printf(" Cliente: %s \n", paqRec);
		    	if(strstr(paqRec, "-fin")){
		    	sendto(udp_socket, "-fin\n",strlen(buffer)+1, 0, (struct sockaddr *)&CLIENTE, sizeof(CLIENTE));
				exit(0);
			}
		}
		//solicito al usuario el mensaje a enviar
			
		printf("Escribir: ");
	    	gets(buffer);
		
		//envio el mensaje.
		env=sendto(udp_socket, buffer,strlen(buffer)+1, 0, (struct sockaddr *)&CLIENTE, sizeof(CLIENTE));
		//COMPROBACION DE ENVIO
		if (env==-1)
		{
		    perror("\n Error al enviar");
		    exit(0);
		}
		else
		{
		    perror("\n -Envio");
		    
		}
        
        }
    }
}
close(udp_socket);
printf("\n");
return 0;
}
