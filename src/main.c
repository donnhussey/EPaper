#include "epaper.h"

UBYTE **img_bufs;
int page_count;

int main(int c, char **v)
{
    char *cvalue;
    int optct;
    int forever_flag = 0;
    int iterations = 0;
    sFONT font = Font12;

    while((optct = getopt(c, v, "cf:r:")) != -1)
    switch(optct){
        case 'c': //clear
            Clear();
            break;
        case 'f': //font
            font = GetFont(optarg);
            break;
        case 'r': //repeat
            iterations = atoi(optarg);
            break;
        default: //default
            forever_flag = 1;
            break;
    }

    if(forever_flag)
        ProcessForever(font);
    else
        ProcessUntil(font, iterations);
}

void ProcessForever(sFONT font)
{
    int max_page_count = 20;
    int max_line_length = (int)(EPD_2IN13_V2_HEIGHT / font.Width); //these are backwards - constants are for portrait mode
    int max_lines = (int)(EPD_2IN13_V2_WIDTH / font.Height);  
    int total_size = max_page_count * max_line_length * max_lines;

    char text[total_size];
    char next_line[max_line_length];
    char page[max_line_length * max_lines];
    UBYTE *pages[max_page_count];

    strcpy(text, "");

    GetInput(total_size, text);

    int page_position = 0;
    int offset = 0;
    page_count = 0;
    do{
        strcpy(page, "");
        for(page_position = 0; page_position < max_lines && offset != -1; page_position++){
                offset = GetNextLine(next_line, text, offset, max_line_length);
                strcat(page, next_line);
        }
        pages[page_count++] = Render(page, &font);
    }while(offset != -1);

    img_bufs = pages;
    DisplayLoopAsync(5);
}

void GetInput(int buf_size, char *input_buf)
{
    char line_buf[buf_size];
    while(fgets(line_buf, buf_size, stdin) != NULL 
     && buf_size > strlen(input_buf) + strlen(line_buf)){
        input_buf = strcat(input_buf, strdup(line_buf));
     }
}

void ProcessUntil(sFONT font, int repeat)
{
    int max_page_count = 20;
    int max_line_length = (int)(EPD_2IN13_V2_HEIGHT / font.Width); //these are backwards - constants are for portrait mode
    int max_lines = (int)(EPD_2IN13_V2_WIDTH / font.Height);  
    int total_size = max_page_count * max_line_length * max_lines;

    char text[total_size];
    char next_line[max_line_length];
    char page[max_line_length * max_lines];
    UBYTE *pages[max_page_count];

    strcpy(text, "");

    GetInput(total_size, text);

    int page_position = 0;
    int offset = 0;
    page_count = 0;
    do{
        strcpy(page, "");
        for(page_position = 0; page_position < max_lines && offset != -1; page_position++){
                offset = GetNextLine(next_line, text, offset, max_line_length);
                strcat(page, next_line);
        }
        pages[page_count++] = Render(page, &font);
    }while(offset != -1);

    img_bufs = pages;
    DisplayLoopAsync(5);
}

UBYTE* Render(char page_content[], sFONT *font)
{
    UBYTE *img_buf;
    UWORD img_buf_size = ((EPD_2IN13_V2_WIDTH % 8 == 0)? (EPD_2IN13_V2_WIDTH / 8 ): (EPD_2IN13_V2_WIDTH / 8 + 1)) * EPD_2IN13_V2_HEIGHT; 

    if((img_buf = (UBYTE *)malloc(img_buf_size)) == NULL) exit(1);
    
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

void Clear()
{
    if(DEV_Module_Init()!=0) exit(1);
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    DEV_Module_Exit();
}

void DisplayLoopAsync(int timeout)
{
/*     int pid = fork();

    if(pid == -1)
        exit(1);
    else if(pid > 0)
        return; */

    signal(SIGINT, Dispose);

    if(DEV_Module_Init()!=0) exit(1); //initialize display
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();

    int current_page = 0;
    UBYTE **img_bufs_cpy;
    img_bufs_cpy = img_bufs;

    while(1)
    {
        if(current_page == page_count) {
            img_bufs_cpy = img_bufs;
            current_page = 0;
        }

        EPD_2IN13_V2_Display(*img_bufs_cpy);
        **img_bufs_cpy++;
        current_page++;
        sleep(timeout);
    }
}

void DisplayAsync(int timeout, int display_loops)
{
    // int pid = fork();

    // if(pid == -1)
    //     exit(1);
    // else if(pid > 0)
    //     return;

    signal(SIGINT, Dispose);

    if(DEV_Module_Init()!=0) exit(1);
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();

    int current_page = 0;
    int current_loop = 0;
    UBYTE **img_bufs_cpy;
    img_bufs_cpy = img_bufs;

    while(current_page < page_count && current_loop < display_loops)
    {
        EPD_2IN13_V2_Display(*img_bufs_cpy);
        **img_bufs_cpy++;
        current_page++;
        current_loop++;
        sleep(timeout);
    }

    int i = 0;
    for(i = 0; i < page_count; i++)
        free(*img_bufs++);

    EPD_2IN13_V2_Clear();  
    DEV_Module_Exit();
    exit(0);
}

sFONT GetFont(char* fontsize)
{
    int size = atoi(fontsize);

    switch(size)
    {
        case 8 : return Font8;
        case 12: return Font12;
        case 16: return Font16;
        case 20: return Font16;
        case 24: return Font24;
        default: return Font12;
    }
}

void  Dispose(int signo)
{
    int i = 0;
    for(i = 0; i < page_count; i++)
        free(*img_bufs++);

    EPD_2IN13_V2_Clear();  
    DEV_Module_Exit();
    exit(0);
}