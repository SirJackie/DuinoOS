#include "GraphicsDriver/DEV_Config.h"
#include "GraphicsDriver/LCD_Driver.h"
#include "GraphicsDriver/LCD_GUI.h"
#include "GraphicsDriver/LCD_Bmp.h"
// #include "GraphicsDriver/LCD_Touch.h"

LCD_SCAN_DIR Lcd_ScanDir = U2D_R2L;    //SCAN_DIR_DFT = D2U_L2R

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

bool isPressed = false;
POINT Xpoint;
POINT Ypoint;

static void TP_Scan()
{
  //In X, Y coordinate measurement, IRQ is disabled and output is low
  if (!digitalRead(TP_IRQ)) {  // PRESSED
    isPressed = true;

    TP_Read_TwiceADC(&Xpoint, &Ypoint);

    // Calculation:
    // X: 3880 -> 320 (originalY)  3560 maps to 480;; 0.135x
    // Y: 280 -> 3780 (originalX)  3500 maps to 320;; 0.091x
    // So: newX = (originalY * -0.135f) + 523.8f
    // So: newY = (originalX * 0.091f) - 25.48f

    float XpointFloat = ((float)Ypoint * -0.135f) + 523.8f;
    float YpointFloat = ((float)Xpoint * 0.091f) - 25.48f;
    Xpoint = (int16_t)XpointFloat;
    Ypoint = (int16_t)YpointFloat;

  } else {  // NOT PRESSED
    isPressed = false;
  }
}



















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
}

void Demo3_Loop()
{
  TP_Scan();

  Serial.println("Milestone 1");

  if (isPressed) {
    Serial.print(Xpoint);
    Serial.print(" ");
    Serial.print(Ypoint);
    Serial.print("\n");
  }

  Serial.println("Milestone 2");

  // GUI_DrawPoint(Xpoint, Ypoint,
  //               BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  // GUI_DrawPoint(Xpoint + 1, Ypoint,
  //               BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  // GUI_DrawPoint(Xpoint, Ypoint + 1,
  //               BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  // GUI_DrawPoint(Xpoint + 1, Ypoint + 1,
  //               BLACK , DOT_PIXEL_1X1, DOT_FILL_RIGHTUP);
  // GUI_DrawPoint(Xpoint, Ypoint,
  //               BLACK , DOT_PIXEL_2X2, DOT_FILL_RIGHTUP);

  Serial.println("Milestone 3");
}

void setup(){
  
  
//  Demo1_Setup();
//  Demo2_Setup();
  Demo3_Setup();
}

void loop(){
//  Demo1_Loop();
//  Demo2_Loop();
  Demo3_Loop();
}
