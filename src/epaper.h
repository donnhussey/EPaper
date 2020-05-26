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
void BuildPages(int max_line_count, int max_line_length, int timeout, int count);
char *GetNextPage(char *input, char *output, int max_line_count);
int CalculateWordLength(const char * str);
void Wrap(char * s, const int wrapline);
char *GetInput(FILE* fp, size_t size);
UBYTE* Render(char page_content[], sFONT *font);
void Clear();
void DisplayAsync(int timeout, int loop_count);
sFONT GetFont(char* fontsize);
void  Dispose(int signo);