#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
int main(){
	udp_socket = socket(AF_INET, SOCK_DGRAM, 0);
	if(udp_socket == -1){
		perror("\nERROR AL ABRIR EL SOCKET\n");
		exit(0);
	}
	else {
		printf("\Exito al abrir el socket");
	}


return 0;

}
