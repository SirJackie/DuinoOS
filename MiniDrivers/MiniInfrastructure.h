#ifndef __MINI_INFRASTRUCTURE_H_
#define __MINI_INFRASTRUCTURE_H_

#include <SPI.h>

/*
** Definitions
*/

// Touch Pin Definitions
#define TP_CS 4
#define TP_CS_0    digitalWrite(TP_CS, LOW)
#define TP_CS_1    digitalWrite(TP_CS, HIGH)

#define TP_IRQ 3
#define GET_TP_IRQ    digitalRead(TP_IRQ)

#define TP_BUSY 6
#define GET_TP_BUSY    digitalRead(TP_BUSY)

/*
** Utilities
*/ 

void SPI4W_Write_Byte(uint8_t DATA);
uint8_t SPI4W_Read_Byte(uint8_t DATA);
void Driver_Delay_us(int xus);

#endif
