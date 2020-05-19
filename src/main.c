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
    //signal(SIGINT, Handler); 
    
    sFONT font = Font12;
    int max_page_count = 5;
    int max_line_length = (int)(EPD_2IN13_V2_WIDTH / font.Width) ;
    int max_lines = (int)(EPD_2IN13_V2_HEIGHT / font.Height);  
    int total_size = max_page_count * max_line_length * max_lines;
    int page_position = 0;
    int page_count = 0;
    char text[total_size];
    char next_line[max_line_length];
    char *input_pos = text;
    char page[max_line_length * max_lines];
    UBYTE *pages[max_page_count];

    strcpy(text, "");
    strcpy(page, "");

    GetInput(total_size, text);

    do{
        for(page_position = 0; page_position < max_lines && *input_pos != '\0'; page_position++){
                input_pos = GetNextLine(next_line, input_pos, max_line_length);
                strcat(page, next_line);
        }
        pages[page_count++] = Render(page, &font);
    }while(*input_pos != '\0');

    printf("%i", page_position);

    int current_page;
    for(current_page = 0; current_page < page_position-1; current_page++){
        Display(pages[current_page]);
        sleep(10);
    }


}

void GetInput(int buf_size, char *input_buf)
{
    char line_buf[buf_size];
    while(fgets(line_buf, buf_size, stdin) != NULL 
     && buf_size > strlen(input_buf) + strlen(line_buf))
        input_buf = strcat(input_buf, strdup(line_buf));
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

char *GetNextLine(char output[], char *input, int max_line_length)
{
    char *start = input;
    int count = 0;

    while(*input != '\n' && *input != '\0' && count++ < max_line_length)
        input++;

    input++;
    count++;

    strncpy(output, start, count);

    if(count-1 == max_line_length){
        count--;
        input -= 2;
        output[count-1] = '\n';
    }

    output[count] = '\0'; //strncopy does not null terminate!
    return input;
}

void Display(UBYTE *img_buf)
{
    if(DEV_Module_Init()!=0) exit(1);
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    EPD_2IN13_V2_Display(img_buf);
}