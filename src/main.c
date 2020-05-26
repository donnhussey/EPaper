#include "epaper.h"

UBYTE **img_bufs;
int page_count;
int persist;

int main(int c, char **v)
{
    int optct;
    int iterations = 0;
    int timeout = 5;
    sFONT font = Font12;
    persist = 0;


    while((optct = getopt(c, v, "cpf:r:t:")) != -1)
    switch(optct){
        case 'c': //clear
            printf("clearing.\n");
            Clear();
            break;
        case 'p': //keep whatever was on the screen on the screen after completion
            printf("persisting.\n");
            persist = 1;
            break;
        case 'f': //font
            font = GetFont(optarg);
            printf("font size %i selected.\n", atoi(optarg));
            break;
        case 'r': //repeat
            printf("Repeating %i times.\n", atoi(optarg));
            iterations = atoi(optarg);
            break;
        case 't': //timeout between screens
            printf("Setting timeout to %i", atoi(optarg));
            timeout = atoi(optarg);
        default: //default
            break;
    }
    

    printf("Iterations: %i", iterations);
    if(iterations > 0)
        ProcessUntil(font, iterations, timeout);
    else
        ProcessForever(font, timeout);
}

void ProcessForever(sFONT font, int timeout)
{
    printf("Printing forever...\n");
    const int max_line_length = (int)(EPD_2IN13_V2_HEIGHT / font.Width); //these are backwards - constants are for portrait mode
    const int max_line_count = (int)(EPD_2IN13_V2_WIDTH / font.Height);  
    UBYTE *pages; 

    printf("Getting input\n");
    char *input = GetInput(stdin, max_line_length);
    char page[max_line_count * max_line_length];
    printf(input);

    printf("\wrapping input\n");
    wrap(input, max_line_length);
    printf(input);
    printf("paging input\n");

    input = NextPage(&input, page, max_line_count, max_line_length);

    while(*input != '\0')
    {
        printf(page);
        printf("\n\n");
        input = NextPage(&input, page, max_line_count, max_line_length);
    }
    //DisplayLoopAsync(timeout);
}

char *NextPage(char *input, char *output, int max_line_count)
{
    int line = 0;
    int pos = 0;

    while(line < max_line_count && input[pos] != '\0'){
        output[pos] = input[pos];      
        if(input[pos] == '\n'){
            printf("newline!\n");
            line++;
        }
        pos++;
    }

    return *input[pos];
}

int wordlen(const char * str){
   int tempindex=0;
   while(str[tempindex]!=' ' && str[tempindex]!=0 && str[tempindex]!='\n'){
      ++tempindex;
   }
   return(tempindex);
}

void wrap(char * s, const int wrapline){

   int index=0;
   int curlinelen = 0;
   while(s[index] != '\0'){
      if(s[index] == '\n'){
         curlinelen=0;
      }
      else if(s[index] == ' '){
         if(curlinelen+wordlen(&s[index+1]) >= wrapline){
            s[index] = '\n';
            curlinelen = 0;
         }
      }
      curlinelen++;
      index++;
   }
}

char *GetInput(FILE* fp, size_t size)
{
    char *str;
    int ch;
    size_t len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp))){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    return realloc(str, sizeof(char)*len);
}

void ProcessUntil(sFONT font, int repeat, int timeout)
{

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

void Clear()
{
    if(DEV_Module_Init()!=0) exit(1);
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();
    DEV_Module_Exit();
}

void DisplayLoopAsync(int timeout)
{
    /*
    int pid = fork();

    if(pid == -1)
        exit(1);
    else if(pid > 0)
        return;
        */

    signal(SIGINT, Dispose);

    if(DEV_Module_Init()!=0) exit(1); //initialize display
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();

    int current_page = 0;
    UBYTE **img_bufs_cpy;
    img_bufs_cpy = img_bufs;

    while(1)
    {
        printf("Attempting to display %i of %i\n", current_page, page_count);
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

void DisplayAsync(int timeout, int loop_count)
{
    /*
     int pid = fork();

     if(pid == -1)
         exit(1);
     else if(pid > 0)
         return;
*/
    signal(SIGINT, Dispose);

    if(DEV_Module_Init()!=0) exit(1);
    EPD_2IN13_V2_Init(EPD_2IN13_V2_FULL);
    EPD_2IN13_V2_Clear();

    int current_page = 0;
    int current_loop = 0;
    UBYTE **img_bufs_cpy;
    img_bufs_cpy = img_bufs;

    while(current_loop < loop_count)
    {
        while(current_page < page_count)
        {
            EPD_2IN13_V2_Display(*img_bufs_cpy);
            printf("displaying page %i of %i, iteration %i of %i\n", current_page, page_count, current_loop, loop_count);
            **img_bufs_cpy++;
            current_page++;
            sleep(timeout);
        }
	img_bufs_cpy = img_bufs;
	current_page = 0; 
        current_loop++;
    }

    int i = 0;
    for(i = 0; i < page_count; i++)
        free(*img_bufs++);

    if(!persist) EPD_2IN13_V2_Clear();  
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

    if(!persist) EPD_2IN13_V2_Clear();  
    DEV_Module_Exit();
    exit(0);
}
