#include "MiniDrivers/MiniInfrastructure.h"
#include "MiniDrivers/MiniTouch.h"
#include "MiniDrivers/MiniGraphics.h"

// #include "GraphicsDriver/DEV_Config.h"
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
  LCD_Init();
  LCD_Clear(WHITE);
  DrawPixel(10, 10, RED);
  DrawChar(80, 80, 'F', &Font16, BLACK);
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
