#include "epaper.h"

void  Handler(int signo)
{
    DEV_Module_Exit();
    exit(0);
}

int main(int c, char **v)
{
    signal(SIGINT, Handler); 
    
    sFONT font = Font12;
    int max_length = EPD_2IN13_V2_WIDTH / font.Width;
    int max_lines = EPD_2IN13_V2_HEIGHT / font.Height;
    int total_size = max_length * max_lines; 
    char text[total_size];
    strcpy(text, "");
    
    //Create a new image cache
    UBYTE *img_buf;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;    
    if((img_buf = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    
    printf("+++++++++1");
    GetInput(max_lines, max_length, &text);
    printf("+++++++++2");
    WriteInput(&img_buf, &text, &font);
    free(img_buf);

    return 0;
}

void GetInput(int max_lines, int max_length, char* inputBuffer)
{
    char line_buf[max_length];
    strcpy(line_buf, "");

    for(int i = 0; i < max_lines && fgets(line_buf, max_length, stdin) != NULL; i++)
        inputBuffer = strcat(inputBuffer, strdup(line_buf));
    printf(inputBuffer);
}

void WriteInput(UBYTE *img_buf, char *text, sFONT *font)
{
    printf("----------------------0-------------------------------");
    if(DEV_Module_Init()!=0)  return -1;
    printf("----------------------1-------------------------------");
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    printf("----------------------2-------------------------------");
    EPD_2IN13_V2_Clear();
    printf("----------------------3-------------------------------");
    Paint_NewImage(&img_buf, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    printf("----------------------4-------------------------------");
    Paint_Clear(WHITE);
    Paint_SetMirroring(MIRROR_HORIZONTAL);
    printf("----------------------5-------------------------------");
    Paint_DrawPage(1, 1, text, font, WHITE, BLACK);
    EPD_2IN13_V2_Display(img_buf);
}