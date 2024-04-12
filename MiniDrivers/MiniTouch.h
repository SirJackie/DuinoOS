#ifndef __MINI_TOUCH_H_
#define __MINI_TOUCH_H_

#include <SPI.h>

// Variables and Functions
extern bool isPressed;
extern int16_t Xpoint, Ypoint;
void Touch();
void TouchInitialize();

#endif
