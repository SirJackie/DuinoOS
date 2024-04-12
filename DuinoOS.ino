#include "MiniDrivers/MiniInfrastructure.h"
#include "MiniDrivers/MiniTouch.h"
#include "MiniDrivers/MiniGraphics.h"

#include <SD.h>

File file;
char counter = 0;

void Demo1_Setup()
{
  Serial.begin(115200);
  Serial.println("Init...");

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
  file = SD.open("hamster.bmp", FILE_READ);
  if (!file) {
    Serial.println("Failed to open file for reading!");
    return;
  }
}

uint8_t fread8(){
  return file.read();
}

uint16_t fread16(){
  return (file.read() << 8) | file.read();
}

uint32_t fread32(){
  return (file.read() << 24) | (file.read() << 16) |
         (file.read() << 8 ) | (file.read()      ) ;
}

void Demo1_Loop()
{
  // Read byte by byte from the file, while file.available().
  while (file.available()) {

    // BMP Header
    file.read();  // Magic Code: 'B'
    file.read();  // Magic Code: 'M'
    file.read();file.read();file.read();file.read();  // uint32_t size;
    file.read();file.read();  // uint16_t reserved1;
    file.read();file.read();  // uint16_t reserver2;
    file.read();file.read();file.read();file.read();  // uint32_t offset;

    // BMP Info Header
    file.read();file.read();file.read();file.read();  // uint32_t size
    file.read();file.read();file.read();file.read();  // uint32


    uint8_t data = file.read();

    // Read a byte
    uint8_t data = file.read();
    
    // Print the byte in hexadecimal format
    if (data < 0x10) {
      Serial.print("0"); // Print leading zero for single-digit values
    }
    Serial.print(data, HEX);
    Serial.print(" ");

    counter++;
    if (counter == 16){
      counter -= 16;
      Serial.print("\n");
    }
  }
  
  // Close the file
  file.close();
  
  // Stop the loop
  while (true) {}
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
  Demo1_Setup();
  // Demo2_Setup();
  // Demo3_Setup();
}

void loop(){
  Demo1_Loop();
  // Demo2_Loop();
  // Demo3_Loop();
}
