#ifndef __MINI_INFRASTRUCTURE_H_
#define __MINI_INFRASTRUCTURE_H_

#include <SPI.h>

/*
** Definitions
*/

// LCD GPIO Pin Definitions
#define LCD_CS 10
#define LCD_CS_0		digitalWrite(LCD_CS, LOW)
#define LCD_CS_1		digitalWrite(LCD_CS, HIGH)

#define LCD_BL 9

#define LCD_RST 8
#define LCD_RST_0		digitalWrite(LCD_RST, LOW)
#define LCD_RST_1		digitalWrite(LCD_RST, HIGH)

#define LCD_DC 7
#define LCD_DC_0		digitalWrite(LCD_DC, LOW)
#define LCD_DC_1		digitalWrite(LCD_DC, HIGH)

// SD GPIO Pin Definitions
#define SD_CS 5
#define SD_CS_0   digitalWrite(LCD_CS, LOW)
#define SD_CS_1    digitalWrite(LCD_CS, HIGH)

// Touch GPIO Pin Definitions
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
void Driver_Delay_ms(unsigned long xms);
void Driver_Delay_us(int xus);

#endif
