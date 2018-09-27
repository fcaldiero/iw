#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

int main (void){
//se crea direccion para el servidor y que escuche  en cualquier direccion
	struct sockaddr_in chat;
	chat.sin_family	     = AF_INET;
	chat.sin_port	     = htons(8080);
	chat.sin_addr.s_addr = INADDR_ANY;
	
//puerto para escuchar 8080
//luego pedimos la funcion socket con AFNET para usar TCP y nos devuelve 1 numero

	int servidor = socket (AF_INET, SOCK_STREAM, 0);


	// esto es para que no se quede colgado el puerto luego de cerrar el servidor

	int activado = 1;
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));		
	
if (bind(servidor,(void*) &chat, sizeof(chat)) !=0 ){
	perror("Fallo el bind\n");
	return 1;
	}
	
	printf("Estoy escuchando\n");
	listen (servidor,1);

//------------------
	unsigned char tecla;
	struct sockaddr_in direccionCliente;
	unsigned int len;
	int cliente = accept (servidor, (void*) &direccionCliente, &len);
	printf("Recibi la conexion en %d!!\n", cliente);
	scanf("%c",&tecla);
	send(cliente,&tecla,1,0);

	for (;;);
	return 0;


}
