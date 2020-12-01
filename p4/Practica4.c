#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <netinet/in.h>

unsigned char MACorigen[6];
unsigned char MACbroad[6]={0xff,0xff,0xff,0xff,0xff,0xff};
unsigned char ethertype[4]={0x0c,0x0c};
unsigned char tramaEnv[1514];

int obtenerDatos (int ds){
    struct ifreq nic;
    char nombre[20];
    int i,index;
    printf("\nInserta el nombre de la interfaz: ");
    scanf("%s", nombre);
    strcpy(nic.ifr_name,nombre);
    if(ioctl(ds,SIOCGIFINDEX,&nic)==-1){
        perror("\n Error al obtener el index");
        exit(0);
    }else{
        index=nic.ifr_ifindex;
        printf("\n El indice es: %d", index);

            if(ioctl(ds,SIOCGIFHWADDR,&nic)==-1){
                perror("\nError al obtener la MAC");
                exit(0);
            }else{
                memcpy(MACorigen, nic.ifr_hwaddr.sa_data,6);
                printf("\n MAC: ");
                for(i=0;i<6;i++){
                    printf("%.2x:", MACorigen[i]);
                }
            }
            //Obterner IP SIOCGIFADDR
	
		if(ioctl(ds,SIOCGIFADDR, &nic)==-1){
			perror("Error al obtener la IP");
		}else {
			memcpy(ethertype, nic.ifr_addr.sa_data,6);
			printf("\n IP: ");
			for(i=0; i<6; i++){
				printf("%2d:",ethertype[i]);
			}
		}
		
		if(ioctl(ds, SIOCGIFNETMASK, &nic)== -1){
			perror("Error al obtener las MASK");
		}else {
			memcpy(MACbroad, nic.ifr_netmask.sa_data,6);
			printf("\n MASK: ");
			for(i=0; i<6; i++){
				printf("%2d:",MACbroad[i]);
			}
		}
		printf("\n");
    }
    return index;
}

void estructuraTrama(unsigned char *trama){
    memcpy(trama+0,MACbroad,6);
    memcpy(trama+6,MACorigen,6);
    memcpy(trama+12,ethertype,2);
    memcpy(trama+14,"Hola, soy Manu", 19);
}

void EnviarTrama (int ds, int index, unsigned char *trama){
    int tam;
    struct sockaddr_ll interfaz;
    memset(&interfaz,0x00, sizeof(interfaz));
    interfaz.sll_family=AF_PACKET;
    interfaz.sll_protocol=htons(ETH_P_ALL);
    interfaz.sll_ifindex=index;
    tam=sendto(ds,trama,60,0,(struct sockaddr *)&interfaz, sizeof(interfaz));
    if(tam==1){
        perror("\nError al enviar");
        exit(0);
    }else perror("\nExito al enviar");
}

int main(){
    int packet_socket, indice;
    packet_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if(packet_socket == -1){
        perror("\nError al abrir el socket");
        exit(0);
    }else{
        perror("\nExito al abrir el socket");
        indice=obtenerDatos(packet_socket);
        estructuraTrama(tramaEnv);
        EnviarTrama(packet_socket,indice,tramaEnv);
    }
    close(packet_socket);
    return 0;
}
