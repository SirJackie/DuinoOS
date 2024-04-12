#ifndef __MINI_GRAPHICS_H_
#define __MINI_GRAPHICS_H_

#include "../GraphicsDriver/LCD_GUI.h"

void LCD_SetWindow(POINT Xstart, POINT Ystart,	POINT Xend, POINT Yend);
void LCD_Write_AllData(uint16_t Data, uint32_t DataLen);
void DrawPixel(uint16_t Xpoint, uint16_t Ypoint, COLOR color);
void DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char AsciiChar, sFONT* Font, COLOR color);

#endif
