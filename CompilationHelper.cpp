// A Hack.
// Arduino has a silly compilation system, that is unable to customize which .cpp file in the sub folder will be compiled.
// So I came up with this idea to make a CompilationHelper.cpp in the same folder where DuinoOS.ino at,
// which efficiently reduced the amount of .cpp file in the root folder. Haha, really interesting solution :)

#include "GraphicsDriver/DEV_Config.cpp"
#include "GraphicsDriver/LCD_Bmp.cpp"
#include "GraphicsDriver/LCD_Driver.cpp"
#include "GraphicsDriver/LCD_GUI.cpp"
#include "GraphicsDriver/LCD_Touch.cpp"
