#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "EPD_2in13_V2.h" 
#include "Debug.h"

#include <stdlib.h> 
#include <signal.h>   
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int main(int c, char **v);
void Process();
char *GetInput(FILE* fp, size_t size);
UBYTE* Render(char *page_content, sFONT *font);
void DisplayLoopAsync(int timeout);
void DisplayAsync(int timeout, int display_loops);
void  Dispose(int signo);
void ProcessForever(sFONT font, int timeout);
void ProcessUntil(sFONT font, int repeat, int timeout);
void Clear();
sFONT GetFont(char* fontsize);
int wordlen(const char * str);
void wrap(char * s, const int wrapline);
char *NextPage(char *input, char *output, int max_line_count);