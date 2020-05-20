#include "epaper.h"
#include <string.h>
#include <stdio.h>

void  Handler(int signo)
{
    DEV_Module_Exit();
    exit(0);
}

int main(int c, char **v)
{
    signal(SIGINT, Handler); 
    
    sFONT font = Font12;
    int max_page_count = 20;
    int max_line_length = (int)(EPD_2IN13_V2_HEIGHT / font.Width); //these are backwards - constants are for portrait mode
    int max_lines = (int)(EPD_2IN13_V2_WIDTH / font.Height);  
    int total_size = max_page_count * max_line_length * max_lines;
    int page_position = 0;
    int page_count = 0;
    char text[total_size];
    char next_line[max_line_length];
    char page[max_line_length * max_lines];
    UBYTE *pages[max_page_count];

    strcpy(text, "");

    GetInput(total_size, text);

    int offset = 0;
        
    do{
        strcpy(page, "");
        for(page_position = 0; page_position < max_lines && offset != -1; page_position++){
                offset = GetNextLine(next_line, text, offset, max_line_length);
                strcat(page, next_line);
        }
        pages[page_count++] = Render(page, &font);
        printf("--page %i rendered-- \n%s\n", page_count, page);
    }while(offset != -1);

    Display(pages, page_count);
}


void GetInput(int buf_size, char *input_buf)
{
    char line_buf[buf_size];
    while(fgets(line_buf, buf_size, stdin) != NULL 
     && buf_size > strlen(input_buf) + strlen(line_buf)){
        input_buf = strcat(input_buf, strdup(line_buf));
     }
}

UBYTE* Render(char page_content[], sFONT *font)
{
    UBYTE *img_buf;
    UWORD Imagesize = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT;    
    if((img_buf = (UBYTE *)malloc(Imagesize)) == NULL) exit(1);
    
    Paint_NewImage(img_buf, EPD_2IN13_V2_WIDTH, EPD_2IN13_V2_HEIGHT, 90, WHITE);
    Paint_SelectImage(img_buf);
    Paint_SetMirroring(MIRROR_HORIZONTAL);
    Paint_Clear(WHITE);
    Paint_DrawString_EN(1, 1, page_content, font, WHITE, BLACK);
    return img_buf;
}

int GetNextLine(char output[], char input[], int input_offset, int max_line_length)
{
    int count = 0;

    while(input[count + input_offset] != '\n' && input[count + input_offset] != '\0' && count < max_line_length-3)
    {
        output[count] = input[input_offset + count];
        count++;
    }

    output[count++] = '\n';
    output[count] = '\0';

    if(input[input_offset + count - 1] == '\0')
        return -1;
    else
        return input_offset + count;
}

void Display(UBYTE *img_bufs[], int page_count)
{
    if(DEV_Module_Init()!=0) exit(1); //initialize display
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);

    int current_page = 0;

    for(current_page = 0; current_page < page_count; current_page++)
    {
        EPD_2IN13_V2_Clear();
        EPD_2IN13_V2_Display(img_bufs[current_page]);
        sleep(5);
    }

}