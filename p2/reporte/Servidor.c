#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>          /*Todas las librerias que se usan en este codigo*/
int main()
{
int udp_socket,lbind,tam,lrecv;  /*Se declaran las variables que se utilizaran en el código*/
struct sockaddr_in SERVIDOR, CLIENTE;  /*Se define la estructura SERVIDOR en CLIENTE que utilizaremos para hacer el intercambio de mensajes*/
unsigned char buffer[100]="Éxito en la terminal ";  /*Definimos un buffer sin signo de cadena de 100 en donde mandamos un mensaje*/
unsigned char paqRec[512];  /*Aqui se definen los caracteres que puedan recibirse.*/
udp_socket = socket(AF_INET, SOCK_DGRAM, 0); /*Hacemos el enlazamiento del socket y marcamos en 0*/

if(udp_socket==-1)
{
    perror("\nError al abrir el socket");
    exit(0);
}
else
{
    perror("\nExito al abrir el socket");      /*Se hace un if para marcar el error o el exito que tuvo el socket al ejecutarse.*/
    SERVIDOR.sin_family=AF_INET; /*Se cambio a SERVIDOR ya que utilizaremos para mandar mensajes en la red local*/
    SERVIDOR.sin_port=htons(8080);   /* Marcamos este numero ya que es para que sea una comunicacion local.*/
    SERVIDOR.sin_addr.s_addr=INADDR_ANY;  /*Le decimos que tome cualquier direccion.*/
    lbind=bind(udp_socket, (struct sockaddr *)&SERVIDOR,sizeof(SERVIDOR)); /*Hacemos el enlazamiento del socket con el SERVIDOR.*/
    if(lbind==-1)
    {
        perror("\n ERROR EN bind");
        exit(0);
    }
    else 
    {
        perror("\n Exito en el bind");  /*Realizamos el proceso del bind con un if para que funcione y nos muestre.*/
        lrecv=sizeof(CLIENTE);
        // Inicio de bucle recibo el mensaje para que podamos verlo en la red mediante el wireshark,
        //
        tam=recvfrom(udp_socket,paqRec, 512, 0, (struct sockaddr *)&CLIENTE,&lrecv);    /*Se pone el tamaño de lo que va a usar el soccket enlazándolo a CLIENTE*/
        if (tam==-1)
        {
            perror("\n Error al recibir");
            exit(0);
        }
        else
        {
            printf("\n El mensaje es :%s", paqRec);
            /*Se verifica si el mensaje se envio o no con un printf.*/
        }        
    }
}
close(udp_socket);
return 0;       /*Se cierra el socket y se termina el código.*/
}




