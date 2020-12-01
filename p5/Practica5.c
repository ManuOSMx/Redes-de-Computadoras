#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> /* the L2 protocols */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ioctl.h>
 #include <net/if.h>

unsigned char MACorigen[6];
unsigned char MACbroad[6]={0xff,0xff,0xff, 0xff,0xff,0xff};
//unsigned char ethertype[2]={0x0c, 0x0c};
//solicitud de ARP
unsigned char ethertype[2]={0x08, 0x06};
unsigned char tramaEnv[1514], tramaRec[1514];
unsigned char IPorigen[4];
unsigned char IPdestino[4];


void imprimirTrama(unsigned char *paq, int len){
    int i;
    for(i=0; i<len; i++){
        if(i%16==0)
        printf("\n");
        printf("%.2x ", paq[i]);
    }
    printf("\n");
}

void estructuraTrama(unsigned char *trama){
    unsigned char tipoh[2]={00,01};
    unsigned char tipoP[2]={0x08,0x00};
    unsigned char lonH[1]={6};
    unsigned char longP[1]={4};
    unsigned char codO[2]={01};
    /*//se estructura la trama
    unsigned char nombre[]="Fabian";
    memcpy(trama+0, MACbroad, 6);
    memcpy(trama+6, MACorigen, 6);
    memcpy(trama+12, ethertype, 2);
    memcpy(trama+14, nombre, 4);*/


    //encabezado MAC
    memcpy(trama+0, MACbroad, 6);
    memcpy(trama+6, MACorigen, 6);
    memcpy(trama+12, ethertype,2);
    //Mesaje de ARP
    
    memcpy(trama+14,tipoh,2);
    memcpy(trama+16,tipoP,2);
    memcpy(trama+18,lonH,1);
    memcpy(trama+19,longP,1);
    memcpy(trama+20,codO,2);
    memcpy(trama+22, MACorigen, 6);
    memcpy(trama+28, IPorigen, 4);
    memset(trama+32, 0x00, 6);
    memcpy(trama+38,IPdestino, 4);
}

void enviarTrama( int ds, int index, unsigned char *trama){
    
    //llenar nuestra estrucutra sockaddr
    int tam;
    struct sockaddr_ll interfaz;
    memset(&interfaz, 0x00, sizeof(interfaz));
    interfaz.sll_family=AF_PACKET;
    interfaz.sll_protocol=htons(ETH_P_ALL);
    interfaz.sll_ifindex=index;
    //se hace el envio
    tam=sendto(ds, trama, 60, 0, (struct sockaddr *)&interfaz, sizeof(interfaz));

    if(tam==-1){
        perror("\nError al enviar");
        exit(0);
    }
    else{
        perror("\nexito al enviar");
    }
}

void recibirTrama(int ds, unsigned char *trama){
    //funcion para chat, recvfrom
    while(1){
        int tam;  
        tam=recvfrom(ds, trama, 60, 0, NULL, 0);
        if(tam==-1){
            perror("\nError al recibir");
        }
        else{
            if(!memcmp(trama+6,MACorigen,6)||!memcmp(trama+0,MACorigen,6));
                imprimirTrama(trama, tam);
                break;
        }
    }  
    
}

int obtenerDatos(int ds){
    struct ifreq nic;
    unsigned char nombre[20];
    int i, index;

    printf("Inserta el nombre de la interfaz: ");
    gets(nombre);

    strcpy(nic.ifr_name, nombre);
    if(ioctl(ds, SIOCGIFINDEX, &nic)==-1){
        perror("\nerror al obtener el index");
    }
    else{ 
        index=nic.ifr_ifindex;
        printf("El indice es %d", index);
        if(ioctl(ds, SIOCGIFHWADDR, &nic)==-1){
            perror("\nErro al obtener la MAC");
            exit(0);
        }
        else{
            memcpy(MACorigen, nic.ifr_hwaddr.sa_data+0, 6);
            printf("\nLa MAC es: ");
            for(i=0; i<6; i++){
                printf("%.2x:", MACorigen[i]);
            }
            printf("hola");
            /*printf("\nLa ip de origen es: ");
            for(i=0;i<6; i++){
                printf("%.2x:", MACorigen[i]);
            }*/
        }
    }
    return index;
}

/*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>><>*/
int main(){



    int packet_socket, indice;
    packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL)); //abrimos nuestro socket

    if(packet_socket==-1){
        perror("Error al abrir el socket");
        exit(0);
    }
    else{
        perror("\nExito al abrir el socket\n");
        indice=obtenerDatos(packet_socket);//se le manda el descriptor de socket
        estructuraTrama(tramaEnv);
        enviarTrama(packet_socket, indice, tramaEnv);
        recibirTrama(packet_socket, tramaRec);
    }

    close(packet_socket); //cerramos nuestro socket

    return 0;
}
