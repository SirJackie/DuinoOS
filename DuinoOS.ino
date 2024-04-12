#include "MiniDrivers/MiniTouch.h"
#include "GraphicsDriver/DEV_Config.h"
#include "GraphicsDriver/LCD_Driver.h"
#include "GraphicsDriver/LCD_GUI.h"
#include "GraphicsDriver/LCD_Bmp.h"

LCD_SCAN_DIR Lcd_ScanDir = U2D_R2L;    //SCAN_DIR_DFT = D2U_L2R

void Demo1_Setup()
{
  System_Init();
  SD_Init();
  LCD_Init(Lcd_ScanDir, 200);
  LCD_Clear(LCD_BACKGROUND);

  LCD_ShowBMP();
}

void Demo1_Loop()
{
  ; // Nothing.
}

void Demo2_Setup()
{
  System_Init();
  LCD_Init(Lcd_ScanDir, 200);
  LCD_Clear(LCD_BACKGROUND);

  GUI_Show();
  DEV_TIME sDev_time;
  sDev_time.Hour = 23;
  sDev_time.Min = 38;
  sDev_time.Sec = 56;

  for (int i = 0; i < 10; i++) {
    sDev_time.Sec++;
    GUI_Showtime(200, 150, 327, 197, &sDev_time, RED);
    Driver_Delay_ms(1000);  //Analog clock 1s
    if (sDev_time.Sec == 60)
      sDev_time.Sec = 0;
  }
}

void Demo2_Loop()
{
  ; // Nothing
}

void Demo3_Setup()
{
  Serial.begin(115200);
  Serial.println("Init...");
  
  System_Init();
  LCD_Init( Lcd_ScanDir, 200);
  LCD_Clear(LCD_BACKGROUND);
  
  TouchInitialize();
}

void Demo3_Loop()
{
  Touch();

  if (isPressed) {
    Serial.print(Xpoint);
    Serial.print(" ");
    Serial.print(Ypoint);
    Serial.print("\n");
  }
}

void setup(){
  // Demo1_Setup();
  // Demo2_Setup();
  Demo3_Setup();
}

void loop(){
  // Demo1_Loop();
  // Demo2_Loop();
  Demo3_Loop();
}
