#include <stdlib.h>    
#include <signal.h>   
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "EPD_Test.h"

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

    sFONT font = Font12;
    if(strlen(text) > GetTotalSize(&font))
        return -1;

    printf(text);
    printf("EPD_2IN13_V2_test Demo\r\n");
    if(DEV_Module_Init()!=0){
        return -1;
    }

    printf("e-Paper Init and Clear...\r\n");
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    DEV_Delay_ms(500);

    //Create a new image cache
    UBYTE *BlackImage;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;    if((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    Paint_NewImage(BlackImage, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    Paint_Clear(WHITE);
    Paint_SetMirroring(MIRROR_HORIZONTAL);
    Paint_DrawPage(1, 1, text, &font, WHITE, BLACK);
    EPD_2IN13_V2_Display(BlackImage);
}

int GetRows(sFONT font){ 
    return EPD_2IN13_V2_HEIGHT / font.Height;
}

int GetCols(sFONT font){
    return EPD_2IN13_V2_WIDTH / font.Width;
}

int GetTotalSize(sFONT font){
    return GetCols(sFONT font) * GetRows(sFONT font);
}
