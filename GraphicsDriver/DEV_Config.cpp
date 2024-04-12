/******************************************************************************
**************************Hardware interface layer*****************************
* | file          :   DEV_Config.
* | version     :   V1.0
* | date        :   2017-12-11
* | function    :
    Provide the hardware underlying interface
******************************************************************************/
#include "DEV_Config.h"
#include <SPI.h>

void PWM_SetValue(uint16_t value)
{
    pinMode(LCD_BL, OUTPUT);
    analogWrite(LCD_BL, value);
}

/********************************************************************************
  function:    Hardware interface
  note:
    SPI4W_Write_Byte(value) :
        hardware SPI
    I2C_Write_Byte(value, cmd):
        hardware I2C
********************************************************************************/
void SPI4W_Write_Byte(uint8_t DATA)
{
  SPI.transfer(DATA);
}
uint8_t SPI4W_Read_Byte(uint8_t DATA)
{
  return SPI.transfer(DATA);
}

/********************************************************************************
  function:    Delay function
  note:
    Driver_Delay_ms(xms) : Delay x ms
    Driver_Delay_us(xus) : Delay x us
********************************************************************************/
void Driver_Delay_ms(unsigned long xms)
{
  delay(xms);
}

void Driver_Delay_us(int xus)
{
  for (int j = xus; j > 0; j--);
}
