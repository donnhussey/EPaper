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

void GetInput(int buf_size, char* inputBuffer);
void Display(UBYTE *img_bufs[], int page_count);
int GetNextLine(char output[], char input[], int input_offset, int max_line_length);
UBYTE* Render(char *page_content, sFONT *font);
