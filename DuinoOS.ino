#include "GraphicsDriver/DEV_Config.h"
#include "GraphicsDriver/LCD_Driver.h"
#include "GraphicsDriver/LCD_GUI.h"
//#include "GraphicsDriver/LCD_Bmp.h"
#include "GraphicsDriver/LCD_Touch.h"

//void Demo1_Setup()
//{
//  System_Init();
//
//  Serial.println("3.5inch TFT Touch Shield ShowBMP Demo");
//  Serial.println("SD_Init ");
//  SD_Init();
//
//  Serial.println("LCD Init ");
//  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;
//  LCD_Init(Lcd_ScanDir, 200);
//
//  Serial.println("LCD_Clear ");
//  LCD_Clear(LCD_BACKGROUND);
//
//  LCD_ShowBMP();
//}
//
//void Demo1_Loop()
//{
//  ; // Nothing.
//}

void Demo2_Setup()
{
  System_Init();

  Serial.println("3.5inch　TFT　Touch Shiled LCD Show...");  
  Serial.println("LCD Init...");
  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;  
  LCD_Init( Lcd_ScanDir, 200);

  Serial.println("LCD_Clear...");
  LCD_Clear(LCD_BACKGROUND);

  Serial.println("LCD_Show...");
  GUI_Show();

  // Show the time
  DEV_TIME sDev_time;
  sDev_time.Hour = 23;
  sDev_time.Min = 38;
  sDev_time.Sec = 56;

  Serial.print("Show time\r\n");
  for (int i = 0; i < 10; i++) {
    sDev_time.Sec++;
    GUI_Showtime(200, 150, 327, 197, &sDev_time, RED);
    Driver_Delay_ms(1000);//Analog clock 1s
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
  System_Init();

  Serial.println("3.5inch TFT Touch Shield Touch Demo");
  Serial.println("Init...");
  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;    //SCAN_DIR_DFT = D2U_L2R
  LCD_Init( Lcd_ScanDir, 200);  

  Serial.println("Init Touch Pad...");
  TP_Init( Lcd_ScanDir );
  TP_GetAdFac();
  TP_Dialog();

  Serial.println("Drawing...");
}

void Demo3_Loop()
{
  TP_DrawBoard();
}

void setup(){
//  Demo1_Setup();
  Demo2_Setup();
  Demo3_Setup();
}

void loop(){
//  Demo1_Loop();
  Demo2_Loop();
  Demo3_Loop();
}
