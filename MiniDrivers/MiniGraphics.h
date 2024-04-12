#ifndef __MINI_GRAPHICS_H_
#define __MINI_GRAPHICS_H_

#include "MiniInfrastructure.h"
#include "../Fonts/fonts.h"

#define	COLOR		   uint16_t		//The variable type of the color (unsigned short)
#define	POINT		   uint16_t		//The type of coordinate (unsigned short)
#define	LENGTH		   uint16_t		//The type of coordinate (unsigned short)

#define WHITE          0xFFFF
#define BLACK          0x0000
#define BLUE           0x001F
#define BRED           0XF81F
#define GRED 		   0XFFE0
#define GBLUE		   0X07FF
#define RED            0xF800
#define MAGENTA        0xF81F
#define GREEN          0x07E0
#define CYAN           0x7FFF
#define YELLOW         0xFFE0
#define BROWN 		   0XBC40
#define BRRED 		   0XFC07
#define GRAY  		   0X8430

#define LCD_WIDTH      480
#define LCD_HEIGHT     320
#define LCD_LENGTH     (uint32_t)153600

void LCD_WriteReg(uint8_t Reg);
void LCD_WriteData(uint8_t Data);

void LCD_Init();
void LCD_SetWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend);
void LCD_Write_AllData(uint16_t Data, uint32_t DataLen);
void LCD_Clear(COLOR color);

void DrawPixel(uint16_t Xpoint, uint16_t Ypoint, COLOR color);
void DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char AsciiChar, sFONT* Font, COLOR color);

#endif
