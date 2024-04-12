#include "MiniInfrastructure.h"

/*
** Utilities
*/ 

void SPI4W_Write_Byte(uint8_t DATA)
{
  SPI.transfer(DATA);
}

uint8_t SPI4W_Read_Byte(uint8_t DATA)
{
  return SPI.transfer(DATA);
}

void Driver_Delay_us(int xus)
{
  for (int j = xus; j > 0; j--);
}
