#include "DEV_Config.h"
#include "GUI_Paint.h"
#include "GUI_BMPfile.h"
#include "EPD_2in13_V2.h" 
#include "Debug.h"

#include <stdlib.h> 
#include <signal.h>   
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int c, char **v);
void Process();
void GetInput(int buf_size, char* inputBuffer);
UBYTE* Render(char *page_content, sFONT *font);
int GetNextLine(char output[], char input[], int input_offset, int max_line_length);
void DisplayLoopAsync(int timeout);
void DisplayAsync(int timeout, int display_loops);
void  Dispose(int signo);
void ProcessForever(sFONT font);
void ProcessUntil(sFONT font, int repeat);
void Clear();
sFONT GetFont(char* fontsize);