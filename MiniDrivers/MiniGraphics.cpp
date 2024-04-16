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

void LCD_Init(){
  // System Init
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  pinMode(LCD_BL,OUTPUT);
  
  // SPI Init
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

  // LCD Hardware Reset
  LCD_RST_1;
  Driver_Delay_ms(500);
  LCD_RST_0;
  Driver_Delay_ms(500);
  LCD_RST_1;
  Driver_Delay_ms(500);

  // Set LCD BackLight to 255 (brightest)
  pinMode(LCD_BL, OUTPUT);
  analogWrite(LCD_BL, 255);

  // Initialize the LCD Registers
  LCD_WriteReg(0XF9);
  LCD_WriteData(0x00);
  LCD_WriteData(0x08);

  LCD_WriteReg(0xC0);
  LCD_WriteData(0x19);//VREG1OUT POSITIVE
  LCD_WriteData(0x1a);//VREG2OUT NEGATIVE

  LCD_WriteReg(0xC1);
  LCD_WriteData(0x45);//VGH,VGL    VGH>=14V.
  LCD_WriteData(0x00);

  LCD_WriteReg(0xC2);	//Normal mode, increase can change the display quality, while increasing power consumption
  LCD_WriteData(0x33);

  LCD_WriteReg(0XC5);
  LCD_WriteData(0x00);
  LCD_WriteData(0x28);//VCM_REG[7:0]. <=0X80.

  LCD_WriteReg(0xB1);//Sets the frame frequency of full color normal mode
  LCD_WriteData(0xA0);//0XB0 =70HZ, <=0XB0.0xA0=62HZ
  LCD_WriteData(0x11);

  LCD_WriteReg(0xB4);
  LCD_WriteData(0x02); //2 DOT FRAME MODE,F<=70HZ.

  LCD_WriteReg(0xB6);//
  LCD_WriteData(0x00);
  LCD_WriteData(0x42);//0 GS SS SM ISC[3:0];
  LCD_WriteData(0x3B);

  LCD_WriteReg(0xB7);
  LCD_WriteData(0x07);

  LCD_WriteReg(0xE0);
  LCD_WriteData(0x1F);
  LCD_WriteData(0x25);
  LCD_WriteData(0x22);
  LCD_WriteData(0x0B);
  LCD_WriteData(0x06);
  LCD_WriteData(0x0A);
  LCD_WriteData(0x4E);
  LCD_WriteData(0xC6);
  LCD_WriteData(0x39);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);
  LCD_WriteData(0x00);

  LCD_WriteReg(0XE1);
  LCD_WriteData(0x1F);
  LCD_WriteData(0x3F);
  LCD_WriteData(0x3F);
  LCD_WriteData(0x0F);
  LCD_WriteData(0x1F);
  LCD_WriteData(0x0F);
  LCD_WriteData(0x46);
  LCD_WriteData(0x49);
  LCD_WriteData(0x31);
  LCD_WriteData(0x05);
  LCD_WriteData(0x09);
  LCD_WriteData(0x03);
  LCD_WriteData(0x1C);
  LCD_WriteData(0x1A);
  LCD_WriteData(0x00);

  LCD_WriteReg(0XF1);
  LCD_WriteData(0x36);
  LCD_WriteData(0x04);
  LCD_WriteData(0x00);
  LCD_WriteData(0x3C);
  LCD_WriteData(0x0F);
  LCD_WriteData(0x0F);
  LCD_WriteData(0xA4);
  LCD_WriteData(0x02);

  LCD_WriteReg(0XF2);
  LCD_WriteData(0x18);
  LCD_WriteData(0xA3);
  LCD_WriteData(0x12);
  LCD_WriteData(0x02);
  LCD_WriteData(0x32);
  LCD_WriteData(0x12);
  LCD_WriteData(0xFF);
  LCD_WriteData(0x32);
  LCD_WriteData(0x00);

  LCD_WriteReg(0XF4);
  LCD_WriteData(0x40);
  LCD_WriteData(0x00);
  LCD_WriteData(0x08);
  LCD_WriteData(0x91);
  LCD_WriteData(0x04);

  LCD_WriteReg(0XF8);
  LCD_WriteData(0x21);
  LCD_WriteData(0x04);

  LCD_WriteReg(0X3A);	//Set Interface Pixel Format
  LCD_WriteData(0x55);

  // Set the Gram Scan Way and Color Transfer Modes of the LCD

  // DOCUMENTATION ABOUT SCAN DIRECTIONS:
  // case L2R_U2D:
  //     MemoryAccessReg_Data = 0x08;
  //     DisFunReg_Data = 0x22;
  // case L2R_D2U:
  //     MemoryAccessReg_Data = 0x08;
  //     DisFunReg_Data = 0x62;
  // case R2L_U2D: //0X4
  //     MemoryAccessReg_Data = 0x08;
  //     DisFunReg_Data = 0x02;
  // case R2L_D2U: //0XC
  //     MemoryAccessReg_Data = 0x08;
  //     DisFunReg_Data = 0x42;
  // case U2D_L2R: //0X2
  //     MemoryAccessReg_Data = 0x28;
  //     DisFunReg_Data = 0x22;
  // case U2D_R2L: //0X6
  //     MemoryAccessReg_Data = 0x28;
  //     DisFunReg_Data = 0x02;
  // case D2U_L2R: //0XA
  //     MemoryAccessReg_Data = 0x28;
  //     DisFunReg_Data = 0x62;
  // case D2U_R2L: //0XE
  //     MemoryAccessReg_Data = 0x28;
  //     DisFunReg_Data = 0x42;

  // We'll do U2D_R2L here:
  uint16_t MemoryAccessReg_Data = 0x28;   // Address: 0x36
  uint16_t DisFunReg_Data = 0x02;         // Address: 0xB6

  // Set the read / write scan direction of the frame memory
  LCD_WriteReg(0xB6);
  LCD_WriteData(0X00);
  LCD_WriteData(DisFunReg_Data);

  LCD_WriteReg(0x36);
  LCD_WriteData(MemoryAccessReg_Data);

  // Waiting for it to transfer & apply
  Driver_Delay_ms(200);
  
  // Sleep Out
  LCD_WriteReg(0x11);
  Driver_Delay_ms(120);

  // Turn on the LCD display
  LCD_WriteReg(0x29);
}

void LCD_SetWindow(uint16_t Xstart, uint16_t Ystart,	uint16_t Xend, uint16_t Yend)
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
void LCD_Write_AllData(uint16_t Data, uint32_t DataLen)
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

void LCD_Clear(COLOR color){
  LCD_SetWindow(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
  LCD_Write_AllData(color, LCD_LENGTH);
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
