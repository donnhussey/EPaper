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
    int max_length = GetRows(&font); //max line length
    int max_lines = GetCols(&font);  //max number of lines
    char text[GetTotalSize(&font)];
    
    GetInput(max_lines, max_length, &text);

    if(DEV_Module_Init()!=0){
        return -1;
    }

    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();

    //Create a new image cache
    UBYTE *img_buf;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;    
    if((img_buf = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }

    Paint_NewImage(img_buf, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    Paint_Clear(WHITE);
    Paint_SetMirroring(MIRROR_HORIZONTAL);
    Paint_DrawPage(1, 1, text, &font, WHITE, BLACK);
    EPD_2IN13_V2_Display(img_buf);
}

int GetRows(sFONT font){ 
    printf("Height: %i\n", font->Height);
    return EPD_2IN13_V2_HEIGHT / font.Height;
}

int GetCols(sFONT font){
    printf("Width: %i\n", font->Width);
    return EPD_2IN13_V2_WIDTH / font.Width;
}

int GetTotalSize(sFONT font){
    return GetCols(font) * GetRows(font);
}

GetInput(int max_lines, int max_length, char* inputBuffer){

    int i;
    char line_buf[max_length];

    for(i = 0; i < max_lines; i++){
        if(fgets(line_buf, max_length, stdin) == NULL)
            break;
        inputBuffer = strcat(inputBuffer, strdup(line_buf));
    } 
}
