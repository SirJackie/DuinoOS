#include "MiniDrivers/MiniInfrastructure.h"
#include "MiniDrivers/MiniTouch.h"
#include "MiniDrivers/MiniGraphics.h"

// #include "GraphicsDriver/DEV_Config.h"
// #include "GraphicsDriver/LCD_Driver.h"
// #include "GraphicsDriver/LCD_GUI.h"
// #include "GraphicsDriver/LCD_Bmp.h"


















// void Demo1_Setup()
// {
//   System_Init();
//   SD_Init();
//   LCD_Init(Lcd_ScanDir, 200);
//   LCD_Clear(LCD_BACKGROUND);

//   LCD_ShowBMP();
// }

// void Demo1_Loop()
// {
//   ; // Nothing.
// }

void Demo2_Setup()
{
  // System_Init();
  //set pin
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_DC, OUTPUT);
  pinMode(LCD_BL,OUTPUT);
  
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

  //Hardware reset
  // LCD_Reset();

  LCD_RST_1;
  Driver_Delay_ms(500);
  LCD_RST_0;
  Driver_Delay_ms(500);
  LCD_RST_1;
  Driver_Delay_ms(500);

  // LCD_SetBackLight(255);

  pinMode(LCD_BL, OUTPUT);
  analogWrite(LCD_BL, 255);

  //Set the initialization register
  // LCD_InitReg();

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

  //Set the display scan and color transfer modes
  // LCD_SetGramScanWay( U2D_R2L);






/********************************************************************************
function:	Set the display scan and color transfer modes
parameter:
		Scan_dir   :   Scan direction
		Colorchose :   RGB or GBR color format
********************************************************************************/


    uint16_t MemoryAccessReg_Data = 0; //addr:0x36
    uint16_t DisFunReg_Data = 0; //addr:0xB6

    // Gets the scan direction of GRAM
    // switch (Scan_dir) {
    // case L2R_U2D:
    //     MemoryAccessReg_Data = 0x08;//0x08 | 0X8
    //     DisFunReg_Data = 0x22;
    //     break;
    // case L2R_D2U:
    //     MemoryAccessReg_Data = 0x08;
    //     DisFunReg_Data = 0x62;
    //     break;
    // case R2L_U2D: //0X4
    //     MemoryAccessReg_Data = 0x08;
    //     DisFunReg_Data = 0x02;
    //     break;
    // case R2L_D2U: //0XC
    //     MemoryAccessReg_Data = 0x08;
    //     DisFunReg_Data = 0x42;
    //     break;
    // case U2D_L2R: //0X2
    //     MemoryAccessReg_Data = 0x28;
    //     DisFunReg_Data = 0x22;
    //     break;
    // case U2D_R2L: //0X6
        MemoryAccessReg_Data = 0x28;
        DisFunReg_Data = 0x02;
        // break;
    // case D2U_L2R: //0XA
    //     MemoryAccessReg_Data = 0x28;
    //     DisFunReg_Data = 0x62;
    //     break;
    // case D2U_R2L: //0XE
    //     MemoryAccessReg_Data = 0x28;
    //     DisFunReg_Data = 0x42;
    //     break;
    // }

    // Set the read / write scan direction of the frame memory
    LCD_WriteReg(0xB6);
    LCD_WriteData(0X00);
    LCD_WriteData(DisFunReg_Data);

    LCD_WriteReg(0x36);
    LCD_WriteData(MemoryAccessReg_Data);















  Driver_Delay_ms(200);

  //sleep out
  LCD_WriteReg(0x11);
  Driver_Delay_ms(120);

  //Turn on the LCD display
  LCD_WriteReg(0x29);

  // LCD_Init(Lcd_ScanDir, 200);
  // LCD_Clear(LCD_BACKGROUND);

  // GUI_DisString_EN(80, 80, "WaveShare Electronic", &Font16, LCD_BACKGROUND, BLUE);
  // GUI_DisChar(80, 80, 'F', &Font16, LCD_BACKGROUND, BLUE);

  DrawPixel(10, 10, RED);
  DrawChar(80, 80, 'F', &Font16, BLACK);

  // LCD_SetPointlColor(0, 0, RED);

  // LCD_SetCursor (Xpoint, Ypoint);
  // LCD_SetColor(Color, 1, 1);

  // LCD_SetWindow(0, 0, 0, 0);
  // LCD_Write_AllData(RED , 1);
}

void Demo2_Loop()
{
  ; // Nothing
}

// void Demo3_Setup()
// {
//   Serial.begin(115200);
//   Serial.println("Init...");
  
//   System_Init();
//   LCD_Init( Lcd_ScanDir, 200);
//   LCD_Clear(LCD_BACKGROUND);
  
//   TouchInitialize();
// }

// void Demo3_Loop()
// {
//   Touch();

//   if (isPressed) {
//     Serial.print(Xpoint);
//     Serial.print(" ");
//     Serial.print(Ypoint);
//     Serial.print("\n");
//   }
// }

void setup(){
  // Demo1_Setup();
  Demo2_Setup();
  // Demo3_Setup();
}

void loop(){
  // Demo1_Loop();
  Demo2_Loop();
  // Demo3_Loop();
}
