#include "MiniDrivers/MiniInfrastructure.h"
#include "MiniDrivers/MiniTouch.h"
#include "MiniDrivers/MiniGraphics.h"
#include "GraphicsDriver/LCD_Bmp.h"

#include <SD.h>

// char BMP_File[BMP_NUM][FILENAME_LEN] =           // add file name here
// {
//   "dog.bmp",
//   "cat.bmp",
//   "hamster.bmp",
//   "rabbit.bmp",
// };

File myFile;

void Demo1_Setup()
{
  LCD_Init();
  LCD_Clear(WHITE);

  // SD_Init();

  SD_CS_1;
  LCD_CS_1;
  TP_CS_1;

  Sd2Card card;
  card.init(SPI_FULL_SPEED, SD_CS);
  if (!SD.begin( SD_CS ))  {
    Serial.println("SD init failed!");
    while (1);                              // init fail, die here
  }
  Serial.println("SD init OK!");

  // Open the file
  myFile = SD.open("hamster.bmp", FILE_READ);

  // If the file fails to open, output an error message
  if (!myFile) {
    Serial.println("Unable to open file");
    return;
  }

  Serial.println("File opened:");

  // Read the file content
  while (myFile.available()) {
    // Read a byte and output it to the serial monitor
    Serial.println((int)myFile.read());
  }

  // Close the file
  myFile.close();

  // LCD_ShowBMP();

}

void Demo1_Loop()
{
  ; // Nothing.
}

void Demo2_Setup()
{
  LCD_Init();
  LCD_Clear(WHITE);
  DrawPixel(10, 10, RED);
  DrawChar(80, 80, 'F', &Font16, BLACK);
  delay(1000);
}

void Demo2_Loop()
{
  ; // Nothing
}

void Demo3_Setup()
{
  Touch_Init();
  LCD_Init();
  LCD_Clear(WHITE);
}

void Demo3_Loop()
{
  Touch();

  if (isPressed) {
    Serial.print(Xpoint);
    Serial.print(" ");
    Serial.print(Ypoint);
    Serial.print("\n");
  }

  DrawPixel(Xpoint, Ypoint, BLACK);
}

void setup(){
  Serial.begin(115200);
  Serial.println("Init...");

  Demo1_Setup();
  // Demo2_Setup();
  // Demo3_Setup();
}

void loop(){
  Demo1_Loop();
  // Demo2_Loop();
  // Demo3_Loop();
}
