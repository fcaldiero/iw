#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <termios.h>
#include <sys/time.h>
#include <stdlib.h>
/////////////////////////////////////////////////////////
static struct termios g_old_kbd_mode; 

// did somebody press somthing??? 
static int kbhit(void){ 
    struct timeval timeout; 
    fd_set read_handles; 
    int status; 

    // check stdin (fd 0) for activity 
    FD_ZERO(&read_handles); 
    FD_SET(0, &read_handles); 
    timeout.tv_sec = timeout.tv_usec = 0; 
    status = select(0 + 1, &read_handles, NULL, NULL, &timeout); 
    return status; 
} 

// put the things as they were befor leave..!!! 
static void old_attr(void){ 
    tcsetattr(0, TCSANOW, &g_old_kbd_mode); 
} 


/////////////////////////////////////////////////////////

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

	struct sockaddr_in direccionCliente;
	unsigned int len;
	int cliente = accept (servidor, (void*) &direccionCliente, &len);
	printf("Recibi la conexion en %d!!\n", cliente);
	
	//uso una llamada al sistema para que ingrese todo lo que escribo sin presionar enter
	


////////////////////////////////////////////////
 char ch; 
    static char init; 
    struct termios new_kbd_mode; 

    if(init) 
        return 0; 
    // put keyboard (stdin, actually) in raw, unbuffered mode 
    tcgetattr(0, &g_old_kbd_mode); 
    memcpy(&new_kbd_mode, &g_old_kbd_mode, sizeof(struct termios)); 
     
    new_kbd_mode.c_lflag &= ~(ICANON | ECHO); 
    new_kbd_mode.c_cc[VTIME] = 0; 
    new_kbd_mode.c_cc[VMIN] = 1; 
    tcsetattr(0, TCSANOW, &new_kbd_mode); 
    atexit(old_attr); 
     
    printf( "Press Any Key ('|' for exit) \n"); 
    while (!kbhit()){ 
        // getting the pressed key... 
        ch = getchar(); 
        send(cliente,&ch,1,0);
	if(ch == '|'){ 
            printf("bye... :)\n"); 
            exit(1); 
        } 
    } 
    return 0; 


}
