#include <stdio.h> 
#include <string.h> 

#include <sys/time.h> 
#include <termios.h>  
#include <stdlib.h> 

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


// main function 
int main( void ){ 
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
     
    printf( "Press Any Key ('q' for exit) \n"); 
    while (!kbhit()){ 
        // getting the pressed key... 
        ch = getchar(); 
        printf("You pressed - %c\n",ch); 
        if(ch == 'q'){ 
            printf("bye... :)\n"); 
            exit(1); 
        } 
    } 
    return 0; 
}  
