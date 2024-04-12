/*****************************************************************************
* | File          :   LCD_Bmp.h
* | Author      :   Waveshare team
* | Function    :   Show SDcard BMP picto LCD 
* | Info        :
*   Image scanning
*      Please use progressive scanning to generate images or fonts
*----------------
* | This version:   V1.0
* | Date        :   2018-01-11
* | Info        :   Basic version
*
******************************************************************************/
#ifndef __LCD_BMP_H__
#define __LCD_BMP_H__

#include "../MiniDrivers/MiniInfrastructure.h"
#include "../MiniDrivers/MiniGraphics.h"

#define MAX_BMP         10                      // bmp file num
#define FILENAME_LEN    20                      // max file name length
#define BMP_NUM        	6



void SD_Init(void);
void LCD_ShowBMP(void);

#endif
