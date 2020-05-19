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

void  Handler(int signo);
void GetInput(int max_lines, int max_length, char* inputBuffer);
void WriteInput(UBYTE *img_buf, char *text, sFONT *font);
