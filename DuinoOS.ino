#include "GraphicsDriver/DEV_Config.h"
#include "GraphicsDriver/LCD_Driver.h"
#include "GraphicsDriver/LCD_GUI.h"
#include "GraphicsDriver/LCD_Bmp.h"
// #include "GraphicsDriver/LCD_Touch.h"




LENGTH LCD_Dis_Column;	//COLUMN
LENGTH LCD_Dis_Page;	//PAGE

POINT LCD_X_Adjust;		//LCD x actual display position calibration
POINT LCD_Y_Adjust;		//LCD y actual display position calibration

LCD_SCAN_DIR Lcd_ScanDir = U2D_R2L;    //SCAN_DIR_DFT = D2U_L2R

POINT Xpoint0;
POINT Ypoint0;
POINT Xpoint;
POINT Ypoint;

unsigned char chStatus;
unsigned char chType;

int iXoff;
int iYoff;
float fXfac;
float fYfac;

#define READ_TIMES  5   //Number of readings
#define LOST_NUM    1   //Discard value

/*******************************************************************************
  function:
        Read the ADC of the channel
  parameter:
    Channel_Cmd :   0x90: Read channel Y +, select the ADC resolution is 12 bits, set to differential mode
                    0xd0: Read channel x +, select the ADC resolution is 12 bits, set to differential mode
*******************************************************************************/
static unsigned int TP_Read_ADC(unsigned char CMD)
{
  unsigned int Data = 0;

  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();

  TP_CS_0;

  SPI4W_Write_Byte(CMD);
  Driver_Delay_us(200);

  Data = SPI4W_Read_Byte(0XFF);
  Data <<= 8;//7bit
  Data |= SPI4W_Read_Byte(0XFF);
  Data >>= 3;//5bit
  TP_CS_1;

  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();

  return Data;
}

static unsigned int TP_Read_ADC_Average(unsigned char Channel_Cmd)
{
  unsigned char i, j;
  unsigned int Read_Buff[READ_TIMES];
  unsigned int Read_Sum = 0, Read_Temp = 0;

  //Read and save multiple samples
  for (i = 0; i < READ_TIMES; i++) {
    Read_Buff[i] = TP_Read_ADC(Channel_Cmd);
    Driver_Delay_us(200);
  }

  //Sort from small to large
  for (i = 0; i < READ_TIMES  -  1; i ++) {
    for (j = i + 1; j < READ_TIMES; j ++) {
      if (Read_Buff[i] > Read_Buff[j]) {
        Read_Temp = Read_Buff[i];
        Read_Buff[i] = Read_Buff[j];
        Read_Buff[j] = Read_Temp;
      }
    }
  }

  //Exclude the largest and the smallest
  for (i = LOST_NUM; i < READ_TIMES - LOST_NUM; i ++)
    Read_Sum += Read_Buff[i];

  //Averaging
  Read_Temp = Read_Sum / (READ_TIMES - 2 * LOST_NUM);

  return Read_Temp;
}

static void TP_Read_ADC_XY(unsigned int *pXCh_Adc, unsigned int  *pYCh_Adc )
{
  *pXCh_Adc = TP_Read_ADC_Average(0xD0);
  *pYCh_Adc = TP_Read_ADC_Average(0x90);
}

#define ERR_RANGE 50    //tolerance scope
static bool TP_Read_TwiceADC(unsigned int *pXCh_Adc, unsigned int  *pYCh_Adc )
{
  unsigned int XCh_Adc1, YCh_Adc1, XCh_Adc2, YCh_Adc2;

  //Read the ADC values Read the ADC values twice
  TP_Read_ADC_XY(&XCh_Adc1, &YCh_Adc1);
  //  Driver_Delay_us(10);
  TP_Read_ADC_XY(&XCh_Adc2, &YCh_Adc2);
  //  Driver_Delay_us(10);

  //The ADC error used twice is greater than ERR_RANGE to take the average
  if ( ((XCh_Adc2 <= XCh_Adc1 && XCh_Adc1 < XCh_Adc2 + ERR_RANGE) ||
        (XCh_Adc1 <= XCh_Adc2 && XCh_Adc2 < XCh_Adc1 + ERR_RANGE))
       && ((YCh_Adc2 <= YCh_Adc1 && YCh_Adc1 < YCh_Adc2 + ERR_RANGE) ||
           (YCh_Adc1 <= YCh_Adc2 && YCh_Adc2 < YCh_Adc1 + ERR_RANGE))) {
    *pXCh_Adc = (XCh_Adc1 + XCh_Adc2) / 2;
    *pYCh_Adc = (YCh_Adc1 + YCh_Adc2) / 2;
    return true;
  }

  //The ADC error used twice is less than ERR_RANGE returns failed
  return false;
}

void TP_GetAdFac(void)
{
  if ( Lcd_ScanDir == D2U_L2R ) { //SCAN_DIR_DFT = D2U_L2R
    fXfac = -0.132443F ;
    fYfac = 0.089997F ;
    iXoff = 516L ;
    iYoff = -22L ;
  } else if ( Lcd_ScanDir == L2R_U2D ) {
    fXfac = 0.089697F ;
    fYfac = 0.134792F ;
    iXoff = -21L ;
    iYoff = -39L ;
  } else if ( Lcd_ScanDir == R2L_D2U ) {
    fXfac = 0.089915F ;
    fYfac =  0.133178F ;
    iXoff = -22L ;
    iYoff = -38L ;
  } else if ( Lcd_ScanDir == U2D_R2L ) {
    fXfac = -0.132906F ;
    fYfac = 0.087964F ;
    iXoff = 517L ;
    iYoff = -20L ;
  } else {
    // LCD_Clear(LCD_BACKGROUND);
    // GUI_DisString_EN(0, 60, "Does not support touch-screen \
    //                     calibration in this direction",
    //                  &Font16, FONT_BACKGROUND, RED);
  }
}

bool isPressed = false;

static void TP_Scan()
{
  //In X, Y coordinate measurement, IRQ is disabled and output is low
  if (!digitalRead(TP_IRQ)) {  // PRESSED
    isPressed = true;

    TP_Read_TwiceADC(&Xpoint, &Ypoint);
    if (Lcd_ScanDir == R2L_D2U) {
      Xpoint = fXfac * Xpoint +
                        iXoff;
      Ypoint = fYfac * Ypoint +
                        iYoff;
    } else if (Lcd_ScanDir == L2R_U2D) {
      Xpoint = LCD_Dis_Column -
                        fXfac * Xpoint -
                        iXoff;
      Ypoint = LCD_Dis_Page -
                        fYfac * Ypoint -
                        iYoff;
    } else if (Lcd_ScanDir == U2D_R2L) {
      Xpoint = fXfac * Ypoint +
                        iXoff;
      Ypoint = fYfac * Xpoint +
                        iYoff;
    } else {
      Xpoint = LCD_Dis_Column -
                        fXfac * Ypoint -
                        iXoff;
      Ypoint = LCD_Dis_Page -
                        fYfac * Xpoint -
                        iYoff;
    }
  } else {  // NOT PRESSED
    isPressed = false;
  }
}



















void Demo1_Setup()
{
  System_Init();

  // Serial.println("3.5inch TFT Touch Shield ShowBMP Demo");
  // Serial.println("SD_Init ");
  SD_Init();

  // Serial.println("LCD Init ");
  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;
  LCD_Init(Lcd_ScanDir, 200);

  // Serial.println("LCD_Clear ");
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

  // Serial.println("3.5inch　TFT　Touch Shiled LCD Show...");  
  // Serial.println("LCD Init...");
  LCD_SCAN_DIR Lcd_ScanDir = SCAN_DIR_DFT;  
  LCD_Init( Lcd_ScanDir, 200);

  // Serial.println("LCD_Clear...");
  LCD_Clear(LCD_BACKGROUND);

  // Serial.println("LCD_Show...");
  GUI_Show();

  // Show the time
  DEV_TIME sDev_time;
  sDev_time.Hour = 23;
  sDev_time.Min = 38;
  sDev_time.Sec = 56;

  // Serial.print("Show time\r\n");
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
  Serial.begin(9600);

  Serial.println("Init...");
  
  System_Init();

  // Serial.println("3.5inch　TFT　Touch Shiled LCD Show...");   
  LCD_Init( Lcd_ScanDir, 200);

  // Serial.println("LCD_Clear...");
  LCD_Clear(LCD_BACKGROUND);

  //Get GRAM and LCD width and height
  if(Lcd_ScanDir == L2R_U2D || Lcd_ScanDir == L2R_D2U || Lcd_ScanDir == R2L_U2D || Lcd_ScanDir == R2L_D2U) {
      LCD_Dis_Column	= LCD_HEIGHT ;
      LCD_Dis_Page = LCD_WIDTH ;
  } else {
      LCD_Dis_Column	= LCD_WIDTH ;
      LCD_Dis_Page = LCD_HEIGHT ;
  }

  // // Serial.println("3.5inch TFT Touch Shield Touch Demo");
  // // Serial.println("Init...");
  // // LCD_Lcd_ScanDir Lcd_ScanDir = U2D_R2L;    //Lcd_ScanDir_DFT = D2U_L2R
  // LCD_Init( Lcd_ScanDir, 200);  

  // // Serial.println("Init Touch Pad...");
  // TP_Init( Lcd_ScanDir );
  // TP_GetAdFac();
  // TP_Dialog();

  // // Serial.println("Drawing...");
}

void Demo3_Loop()
{
  TP_GetAdFac();
  TP_Scan();

  GUI_DrawPoint(Xpoint, Ypoint,
                BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  GUI_DrawPoint(Xpoint + 1, Ypoint,
                BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  GUI_DrawPoint(Xpoint, Ypoint + 1,
                BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  GUI_DrawPoint(Xpoint + 1, Ypoint + 1,
                BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  GUI_DrawPoint(Xpoint, Ypoint,
                BLACK , DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);

  // TP_DrawBoard();
}

void setup(){
  
  
//  Demo1_Setup();
//  Demo2_Setup();
  Demo3_Setup();
}

void loop(){
//  Demo1_Loop();
//  Demo2_Loop();
//  Demo3_Loop();
}
