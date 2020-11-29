#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> /* superset of previous */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>                      /*Las librerias que usaremos en este ccódigo.*/
int main()
{
int udp_socket,lbind,tam;               /*Definimos las variables que se usarán más adelante.*/
struct sockaddr_in local, remota;	/*Definimos una estructura local.*/	
unsigned char buffer[512]="Éxito en la terminal"; //el bufer lo cambio a 512 para que vaya el mensaje a donde esta el servidor
udp_socket = socket(AF_INET, SOCK_DGRAM, 0); //Igualamos en la red 

if(udp_socket==-1)
{
    perror("\nError al abrir el socket");
    exit(0);                        /*Hacemos el if para que nos diga si hay o no hay un error al abrir el socket*/
}
else
{
    perror("\nExito al abrir el socket");
    local.sin_family=AF_INET; /* address family: AF_INET; se designan las familias locales de la red */ 
    local.sin_port=htons(0);   /* port in network byte order; Se declara el puerto y se pone en cero porque es la parte del cliente. */
    local.sin_addr.s_addr=INADDR_ANY;    /*Esto nos menciona que direccion tomar.*/
    lbind=bind(udp_socket, (struct sockaddr *)&local,sizeof(local)); /*Se realiza un enlazamiento con en socket de la estructura local en este caso de Costumer*/
    if(lbind==-1)
    {
        perror("\n ERROR EN bind");
        exit(0);
    }
    else 
    {
        perror("\n Exito en el bind");     /*Se realiza el if para que nos mande si hay o no hay error en el bind y poder seguir con el procedimiento.*/
        remota.sin_family=AF_INET; /* address family: AF_INET; Con remota se declara el puerto de las familias. */
        remota.sin_port=htons(8080);   /* port in network byte order, cambio puerto serv; Aqui va a tomar la direccion que se le esta mencionando en este caso la 8080 porque será una red local. */
        remota.sin_addr.s_addr=inet_addr("127.0.0.1");  /*En esta linea se hace el enlazamiento de nuestra red.*/
    //iniciar ciclo while o dowhile
    // capturar el mensaje con un fgets o con gets entonces envio mensaje
        tam=sendto(udp_socket, buffer,strlen(buffer)+1, 0, (struct sockaddr *)&remota, sizeof(remota));  /*Se le agrega la variable de tamaño, diciendole la estructura, el buffer, haciendo un cast para enlazarlo con remota*/
        if (tam==-1)
        {
            perror("\n Error al enviar");
            exit(0);
        }
        else
        {
            perror("\n Exito al enviar"); 
            // Se envia el mensaje a la red del servidor para que sea exitoso o puede mandar un error y no enviarlo.
        }
        
    }
}
close(udp_socket);
return 0;          //Cerramos el socket y el código
}
