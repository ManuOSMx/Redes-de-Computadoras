#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

int main()
{
time_t horario;
int udp_socket,lbind,env,rec,lrecv;
struct sockaddr_in local, remota;
unsigned char buffer[512]; //el bufer lo cambio a 512
unsigned char paqRec[512];
udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

if(udp_socket==-1)
{
    perror("\nError al abrir el socket");
    exit(0);
}
else
{
    perror("\nExito al abrir el socket");
    local.sin_family=AF_INET; /* address family: AF_INET */
    local.sin_port=htons(0);   /* port in network byte order */
    local.sin_addr.s_addr=INADDR_ANY;
    lbind=bind(udp_socket, (struct sockaddr *)&local,sizeof(local));
    if(lbind==-1)
    {
        perror("\n ERROR EN bind");
        exit(0);
    }
    else 
    {
        perror("\n Exito en el bind");
        remota.sin_family=AF_INET; /* address family: AF_INET */
        remota.sin_port=htons(8080);   /* port in network byte order, cambio puerto serv */
        remota.sin_addr.s_addr=inet_addr("172.17.0.1");
        
    //iniciar ciclo while o dowhile
    printf("==========INICIAR CHAT==============");
	while(!strstr(buffer, "-fin") || !strstr(paqRec, "-fin")){
	
		time(&horario);
		struct tm *tiempo = localtime(&horario);		
		
		printf("\nEscribir: ");
		gets(buffer);	// capturar el mensaje
		env=sendto(udp_socket, buffer,strlen(buffer)+1, 0, (struct sockaddr *)&remota, sizeof(remota)); //Se envia el mensaje al servidor
		
		if (env==-1)
		{
		    perror("\n Error al enviar");
		    exit(0);
		}
		else
		{
		    perror("\n -Envio");
		    
		}
		
		// Recibir el mensaje de respuesta 
		rec=recvfrom(udp_socket,paqRec, 512, 0, (struct sockaddr *)&remota,&lrecv);
		//COMPROBACION DE RECIBO
		if (rec==-1)
		{
			perror("\n Error al Recibir");
			exit(0);
		}
		else
		{
			// imprimo el mensaje
		
			printf("\n[%d:%d]",tiempo->tm_hour, tiempo->tm_min);
			printf(" SERVIDOR: %s", paqRec);
			if(strstr(paqRec, "-fin")){
				sendto(udp_socket, "-fin\n",strlen(buffer)+1, 0, (struct sockaddr *)&remota, sizeof(remota));
				exit(0);
			}
		}
				
        }
        
    }
}
close(udp_socket);
printf("\n\n");
return 0;
}
