#include "MiniInfrastructure.h"
#include "MiniTouch.h"

/*
** Definitions
*/

#define READ_TIMES  5   //Number of readings
#define LOST_NUM    1   //Discard value

/*
** Private Functions
*/

static unsigned int TP_Read_ADC(unsigned char CMD)
{
  // Read the ADC of the channel

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
  // 0x90: Read channel Y +, select the ADC resolution is 12 bits, set to differential mode
  // 0xd0: Read channel x +, select the ADC resolution is 12 bits, set to differential mode
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

/*
** Variables
*/

bool isPressed = false;
int16_t Xpoint;
int16_t Ypoint;

/*
** Public Functions
*/

static void Touch()
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

  SPI.transfer(0x2C);  // Magik Code (0x2C or 0x2B), without this the touch screen will crash.
  // I have checked the manual, and have no idea how this could be affecting the touch screen
  // since the touch screen is already unattached (disabled) earlier in the TP_CS_1; (digitalWrite(TP_CS, HIGH))
}

static void Touch_Init() {
  pinMode(TP_CS, OUTPUT);
  pinMode(TP_IRQ, INPUT);
  digitalWrite(TP_IRQ, HIGH);
  
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.begin();
}
