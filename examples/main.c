#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include "EPD_Test.h"   //Examples

void  Handler(int signo)
{
    printf("\r\nHandler:exit\r\n");
    DEV_Module_Exit();
    exit(0);
}

int main(void)
{
    signal(SIGINT, Handler);    
    EPD_2in13_V2_test();
    return 0;
}
