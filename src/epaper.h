#include "../lib/Config/DEV_Config.h"
#include "../lib/GUI/GUI_Paint.h"
#include "../lib/GUI/GUI_BMPfile.h"
#include "../lib/e-Paper/EPD_2in13_V2.h" 
#include "../lib/config/Debug.h"

#include <stdlib.h> 
#include <signal.h>   
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int c, char **v);
void GetInput(int buf_size, char* inputBuffer);
UBYTE* Render(char *page_content, sFONT *font);
int GetNextLine(char output[], char input[], int input_offset, int max_line_length);
void Display(int timeout);
void  Dispose(int signo);
