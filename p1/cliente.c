#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


int main()
{
int udp_socket,lbind,tam; //Declaramos el socket, lbind (Enlazar el programa)
struct sockaddr_in local, remota; //Inicializamos la estructura (formato de la estructura del manual
unsigned char buffer[100]="Hola Manu"; //Mandara un mensaje a la Red.
udp_socket = socket(AF_INET, SOCK_DGRAM, 0); //Recomendacion del manual

/*
Se hará una comprobación en la inicializacion del socket, si da -1 se sale del programa.
en caso contrario, comenzara el programa. 
*/
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
    local.sin_addr.s_addr=INADDR_ANY;// internet address
    lbind=bind(udp_socket, (struct sockaddr *)&local,sizeof(local));//Se hace un cast de la estructura socketaddr lo enlace con local,el buffer es variable local
    
//Comprobamos que no haya problema con el lbind
    if(lbind==-1)
    {
        perror("\n ERROR EN bind");
        exit(0);
    }
    else 
    {
        perror("\n Exito en el bind");
        remota.sin_family=AF_INET; /* address family: AF_INET */
        remota.sin_port=htons(53);   /* port in network byte order */
        remota.sin_addr.s_addr=inet_addr("8.8.8.8");
        tam=sendto(udp_socket, buffer, 100, 0, (struct sockaddr *)&remota, sizeof(remota));//cast de sockaddr para que lo enlace con remota
        if (tam==-1)
        {
            perror("\n Error al enviar");
            exit(0);
        }
        else
        {
            perror("\n Exito al enviar");
        }
        
    }
}
//Cerramos el socket
close(udp_socket);
return 0;
}
