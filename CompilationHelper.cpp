/*
** A Hack.
** Arduino has a silly compilation system, that is unable to customize which .cpp file
** in the sub folder will be compiled. So I came up with this idea to make a CompilationHelper.cpp
** in the same folder where DuinoOS.ino at,which efficiently reduced the amount of .cpp file
** in the root folder. Haha, really interesting solution :)
** Side Effect:
** Whatever .cpp included here will have a slight folder confusion that is needed to fixed manually.
*/

// MiniDrivers
#include "MiniDrivers/MiniInfrastructure.cpp"
#include "MiniDrivers/MiniTouch.cpp"
#include "MiniDrivers/MiniGraphics.cpp"

// Fonts
#include "Fonts/font16.c"
