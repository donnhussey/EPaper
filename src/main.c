#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void  Handler(int signo)
{
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();
    exit(0);
}

int main(int c, char **v)
{
    signal(SIGINT, Handler); 
    
    while(opt = getopt(c, v, "af")) != -1)
        switch(c)
        {
            case 'c':
                //clear
                return 0;
            case 'f':
                //set font
                break;
            default:
                break;
        }
        
    for(int i = optind; i < argc; i++)
        printf(v[i]);
        
    return 0;
}
