#include "MiniGraphics.h"

/*******************************************************************************
function:
		Write register address and data
*******************************************************************************/
void LCD_WriteReg(uint8_t Reg)
{
    LCD_DC_0;
    LCD_CS_0;
    SPI4W_Write_Byte(Reg);
    LCD_CS_1;
}

void LCD_WriteData(uint8_t Data)
{
    LCD_DC_1;
    LCD_CS_0;
    SPI4W_Write_Byte(Data >> 8);
    SPI4W_Write_Byte(Data & 0XFF);
    LCD_CS_1;
}

/********************************************************************************
function:	Sets the start position and size of the display area
parameter:
	Xstart 	:   X direction Start coordinates
	Ystart  :   Y direction Start coordinates
	Xend    :   X direction end coordinates
	Yend    :   Y direction end coordinates
********************************************************************************/
static void LCD_SetWindow(uint16_t Xstart, uint16_t Ystart,	uint16_t Xend, uint16_t Yend)
{
    //set the X coordinates
    LCD_WriteReg(0x2A);
    LCD_WriteData(Xstart >> 8);	 				//Set the horizontal starting point to the high octet
    LCD_WriteData(Xstart & 0xff);	 				//Set the horizontal starting point to the low octet
    LCD_WriteData((Xend - 1) >> 8);	//Set the horizontal end to the high octet
    LCD_WriteData((Xend - 1) & 0xff);	//Set the horizontal end to the low octet

    //set the Y coordinates
    LCD_WriteReg(0x2B);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart & 0xff );
    LCD_WriteData((Yend - 1) >> 8);
    LCD_WriteData((Yend - 1) & 0xff);
    LCD_WriteReg(0x2C);
}

/*******************************************************************************
function:
		Write register data
*******************************************************************************/
static void LCD_Write_AllData(uint16_t Data, uint32_t DataLen)
{
    uint32_t i;
    LCD_DC_1;
    LCD_CS_0;
    for(i = 0; i < DataLen; i++) {
        SPI4W_Write_Byte(Data >> 8);
        SPI4W_Write_Byte(Data & 0XFF);
    }
    LCD_CS_1;
}

void DrawPixel(uint16_t Xpoint, uint16_t Ypoint, COLOR color){
    LCD_SetWindow(Xpoint, Ypoint, Xpoint, Ypoint);
    LCD_Write_AllData(color, 1);
}

void DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char AsciiChar, sFONT* Font, COLOR color){
  uint32_t Char_Offset = (AsciiChar - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
  const unsigned char *ptr = &Font->table[Char_Offset];

  for (uint16_t Page  = 0; Page < Font->Height; Page ++ ) {
    for (uint16_t Column = 0; Column < Font->Width; Column ++ ) {
        // 10000000 >> xOffset
        if (pgm_read_byte(ptr) & (0x80 >> (Column % 8))) {
          DrawPixel(Xpoint + Column, Ypoint + Page, color);
        }
        if (Column % 8 == 7) {
          ptr++;
        }
    }
    if (Font->Width % 8 != 0) {
      ptr++;
    }
  }
}
