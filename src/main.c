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
        
        
    int max_length = 200;
    int max_lines = 200;
        
    char* input[max_size];
    
    if(optind == c)
    {
        get_input(max_size, max_lines, input);
        puts(input);
    }
    else
        for(int i = optind; i < c; i++)
            puts(v[i]);
        
    return 0;
}

int get_input(int max_length, int max_lines, char **input)
{
    char line_buf[max_length];
    int i;
    
    for(i = 0; i < max_lines; i++){
        fgets(line_buf, max_length, stdin);
        input[i] = strdup(line_buf);
    }
}
