/******************************************************************************
**************************Hardware interface layer*****************************
* | file          :   DEV_Config.h
* | version     :   V1.0
* | date        :   2017-12-11
* | function    :
    Provide the hardware underlying interface
******************************************************************************/
#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <SPI.h>
#include <Wire.h>

#define USE_SPI_4W 1
#define USE_IIC    0



// // NON COMPILABLE
// #define SPI4W_Write_Byte(__DATA) SPI.transfer(__DATA)
// #define SPI4W_Read_Byte(__DATA) SPI.transfer(__DATA)

/*------------------------------------------------------------------------------------------------------*/
uint8_t System_Init(void);
void PWM_SetValue(uint16_t value);
void SPI4W_Write_Byte(uint8_t DATA);
uint8_t SPI4W_Read_Byte(uint8_t DATA);

void Driver_Delay_ms(unsigned long xms);
void Driver_Delay_us(int xus);

#endif
