#include <stdlib.h>    
#include <signal.h>   
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
    
    int max_length = 200; //max line length
    int max_lines = 200;  //max number of lines
    char* input[max_lines]; //location to store input
    char line_buf[max_length]; //line buffer
    int i; //array counter
    int final_line_count = max_length; //total number of lines initialized to max
    char* text; //text to display

    for(i = 0; i < max_lines; i++){
        if(fgets(line_buf, max_length, stdin) == NULL) {
            final_line_count = i;
            break;
        }
        input[i] = strdup(line_buf);
    } 

    text = input[0];
    for(i = 1; i < final_line_count; i++){
        strcat(text, input[i]);
    }

    //initialize the display
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();

    //Create a new image cache
    UBYTE *BlackImage;
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;
    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("cannot allocate mempry, fatal error...\r\n");
        return -1;
    }

    //paint white to the new image
    Paint_NewImage(BlackImage, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    Paint_SelectImage(BlackImage);
    Paint_SetMirroring(MIRROR_HORIZONTAL);
    Paint_Clear(WHITE);
    EPD_2IN13_V2_Display(BlackImage);

    Paint_DrawString_EN(140, 15, text, &Font16, BLACK, WHITE);
     DEV_Module_Exit();
     free(BlackImage);
    return 0;
}
