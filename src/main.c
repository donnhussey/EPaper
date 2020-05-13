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
    
    int o;
    
    while((o = getopt(c, v, "cf:")) != -1)
        switch(o)
        {
            case 'c':
                printf("clearing screen\n");
                return 0;
            case 'f':
                printf("setting font\n");
                printf(optarg);
                break;
            default:
                break;
        }
        
    if(optind == c)
        printf(get_input(1000));
    else
        for(int i = optind; i < c; i++)
            printf(v[i]);
        
    return 0;
}

char *get_input(int max_size)
{
    char fullinput[max_size];
    while(fgets(fullinput, max_size, stdin) != null);
    return fullinput;
}
