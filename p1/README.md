Practica 1

Aquí se pidió hacer un socket sencillo.
El mensaje lo puedes cambiar en el unsigned char buffer[512]="Hola Manu"; y escribir el mensaje que quieres enviar.
Es importante que cambies la IP para que funcione.
colocas tu IP en 
remota.sin_addr.s_addr=inet_addr("Coloca aqui tu IP");

Posteriormente compilaras y ejecutaras el cliente.c, de igual forma tienes que esperar hasta recibir un mensaje de "Succes".
Esta practica tiene como fin Mandar un mensaje en la red para que WireShark pueda detectarlo
