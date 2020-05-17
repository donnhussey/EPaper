#include "epaper.h"
//try again...6

void  Handler(int signo)
{
    DEV_Module_Exit();
    exit(0);
}

int main(int c, char **v)
{
    signal(SIGINT, Handler); 
    
    sFONT font = Font12;
    int total_size = GetBufferLength(&font);
    char text[total_size];
    char **pages;
    strcpy(text, "");
    int page_count = 0;
    
    UBYTE *img_buf;
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;    
    if((img_buf = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
        return -1;
    }
    
    GetInput(max_lines, max_length, &text);
    printf(text);
    //pages = PageInput(total_size, 5, text);
    //PrintPagedInput(5, pages);
    //WriteInput(img_buf, text, &font);
    free(img_buf);
    DEV_Module_Exit();
    return 0;
}

void GetInput(int buf_size, char* inputBuffer)
{
    char line_buf[buf_size];
    for(fgets(line_buf, max_length, stdin) != NULL 
     && buf_size < strlen(inputBuffer) + strlen(line_buf))
        inputBuffer = strcat(inputBuffer, strdup(line_buf));
}


char** PageInput(int bufferLength, int max_pages, char *input)
{
    int i, j;
    char *paged_input[max_pages];

    for (i = 0; i < max_pages && input != '\0'; i++){ //page loop

        if(!paged_input[i] = malloc(buffer_length * sizeof(char)))
            exit(-1);

        for(j = 0; j < bufferLength; j++){ //character loop
            paged_input[i][j] = input;
            
            if(input == '\0')
                break;

            *input++;
        }
    }
}

void PrintPagedInput(int maxPages, char **text){

    int i;
    for(i = 0; i < maxPages, i++)
        printf(text[i]);
}
 
int GetBufferLength(sFONT *font){
    return  (int)((EPD_2IN13_V2_WIDTH / font.Width) * (EPD_2IN13_V2_HEIGHT / font.Height));  
}

void WriteInput(UBYTE *img_buf, char *text, sFONT *font)
{
    if(DEV_Module_Init()!=0) return -1;
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    Paint_NewImage(img_buf, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 270, WHITE);
    Paint_SelectImage(img_buf);
    Paint_SetMirroring(MIRROR_HORIZONTAL);
    Paint_Clear(WHITE);
    Paint_DrawPage(1, 1, text, font, WHITE, BLACK);
    EPD_2IN13_V2_Display(img_buf);
}