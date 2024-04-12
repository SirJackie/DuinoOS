/*****************************************************************************
* | File          :   LCD_Bmp.c
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
#include <SD.h>
#include <SPI.h>

#include "LCD_Bmp.h"



char BMP_File[BMP_NUM][FILENAME_LEN] =           // add file name here
{
  "dog.bmp",
  "cat.bmp",
  "hamster.bmp",
  "rabbit.bmp",
};





static void LCD_DrawBmp( int x, int y)
{
  
}
/**************************************************************************
    This procedure reads a bitmap and draws it to the screen
    its sped up by reading many pixels worth of data at a
    time instead of just one pixel at a time. increading the buffer takes
    more RAM but makes the drawing a little faster. 20 pixels' worth
    is probably a good place
**************************************************************************/
void SD_Init(void) 
{
  
}

void LCD_ShowBMP(void) 
{
  unsigned char i;
  for (i = 0; i < BMP_NUM; i++) {
    bmpFile = SD.open(BMP_File[i]);
    if (! bmpFile) {
      // DEBUG(BMP_File[i]);
      // DEBUG(" image not found");
      continue ;
    } else {
      // DEBUG(BMP_File[i]);
      // DEBUG(" image open");
    }

    
    bmpFile.close();
    
    delay(1000);
    delay(1000);
  }
}

