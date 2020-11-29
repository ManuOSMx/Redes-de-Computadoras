#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <netinet/in.h>

unsigned char MAC[6], MASK[4], IP[4];

void obtenerDatos(int ds){ //ds= Descriptor de socket
	
	int i= 0;
	char nombre[10];
	struct ifreq nic; //man netdevice
	printf("\nIntroduce la interfaz de red: ");
	scanf("%s", nombre);
	strcpy(nic.ifr_name, nombre);
	
	//Obtener el indice
	if(ioctl(ds,SIOCGIFINDEX, &nic)==-1){
		perror("Error al obtener el indice");
	}
	else{
		printf("\nEl indice es: %d\n", nic.ifr_ifindex);
	}

	//Obtener MAC SIOCGIFWADDR
	if(ioctl(ds,SIOCGIFHWADDR,&nic)==-1){
		perror("Error al obtener la MAC");
	}
	else{
		memcpy(MAC,nic.ifr_hwaddr.sa_data,6);
		printf("\nMAC:");
		for(i=0;i<6;i++){
			printf("%2x:", MAC[i]);
		}
	}
	
	//Obterner IP SIOCGIFADDR
	
	if(ioctl(ds,SIOCGIFADDR, &nic)==-1){
		perror("Error al obtener la IP");
	}else {
		memcpy(IP, nic.ifr_addr.sa_data,6);
		printf("\nIP: ");
		for(i=0; i<6; i++){
			printf("%2d: ",IP[i]);
		}
	}
	
	if(ioctl(ds, SIOCGIFNETMASK, &nic)== -1){
		perror("Error al obtener las MASK");
	}else {
		memcpy(MASK, nic.ifr_netmask.sa_data,6);
		printf("\nMASK: ");
		for(i=0; i<6; i++){
			printf("%2d: ",MASK[i]);
		}
	}
} 

int main(){
	int packet_socket;
	packet_socket= socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if(packet_socket == -1){
		perror("Error al abrir el socket");
		exit(1);
		}
	else{
		perror("Exito al abrir el socket");
		obtenerDatos(packet_socket);
	}
	close(packet_socket);
	printf("\n");
}
