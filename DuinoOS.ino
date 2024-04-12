#include "MiniDrivers/MiniInfrastructure.h"
#include "MiniDrivers/MiniTouch.h"
#include "MiniDrivers/MiniGraphics.h"
// #include "GraphicsDriver/LCD_Bmp.h"

#include <SD.h>

#define BUFFPIXEL_X3(__val)    ( (__val) * 3)                 // BUFFPIXELx3
#define RGB24TORGB16(R,G,B) (( (R) >> 3 ) << 11 ) | (( (G) >> 2 ) << 5) | ( (B) >> 3)

#define clamp0(x) (x > 0 ? x : 0)
#define clamp255(x) (x < 255 ? x : 255)
#define clampChannel(x) (clamp255(clamp0(x)))

typedef struct {
  uint32_t Size;
  uint32_t Index ;
  uint32_t Header_Size ;
  uint32_t Bit_Pixel ;
  POINT BMP_Width;
  POINT BMP_Height;
} BMP_DIS;

BMP_DIS BMP_Dis;

/*********************************************/
// These read data from the SD card file and convert them to big endian
// (the data is stored in little endian format!)
static uint16_t Read16(File f)
{
  uint16_t d;
  uint8_t b;
  b = f.read();
  d = f.read();
  d <<= 8;
  d |= b;
  return d;
}

// LITTLE ENDIAN!
static uint32_t Read32(File f)
{
  uint32_t d;
  uint16_t b;

  b = Read16(f);
  d = Read16(f);
  d <<= 16;
  d |= b;
  return d;
}

static boolean ReadBmpHeader(File f)
{
  uint16_t BMP_File;
  BMP_File = Read16(f) ;//0000h 2byte: file type
  
  if (BMP_File != 0x4D42) {
    // magic bytes missing
    return false;
  }

  // read file size
  BMP_Dis.Size = Read32(f);//0002h 4byte: file size

  // read and ignore creator bytes
  Read32(f);//0006h 4byte:

  BMP_Dis.Index = Read32(f);//000ah 4byte: Offset between file head and image (offbits)

  // read DIB header
  BMP_Dis.Header_Size = Read32(f);//000Eh 4byte: Bitmap header = 40

  BMP_Dis.BMP_Width = Read32(f);//12-15:Image wide
  BMP_Dis.BMP_Height = Read32(f);//16-19:Image high

  // if (BMP_Dis.BMP_Width != sLCD_DIS.LCD_Dis_Column || BMP_Dis.BMP_Height != sLCD_DIS.LCD_Dis_Page)  {   // if image is not 480*320, return false
  //   // DEBUG("BMP is not 480*320");
  //   return false;
  // }

  if (Read16(f) != 1)//1A-1B :0 for the use of the palette, 1 for the number of color indices
    return false;

  BMP_Dis.Bit_Pixel = Read16(f);//1C-1D：Whether the image is compressed

  if (Read32(f) != 0) {//1E-21：
    // compression not supported!
    return false;
  }

  return true;
}

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
  // LCD_Clear(WHITE);

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

  // // Read the file content
  // while (myFile.available()) {
  //   // Read a byte and output it to the serial monitor
  //   Serial.println((int)myFile.read());
  // }

  

  // LCD_ShowBMP();

  if (! ReadBmpHeader(myFile)) {
    // ERROR BMP FORMAT
    ;
  }

  // LCD_DrawBmp(0, 0);


  myFile.seek( BMP_Dis.Index);
  uint16_t BuffPixel =  BMP_Dis.BMP_Width / 4;
  uint16_t X_Point = (uint16_t)0;
  uint16_t Y_Point = (uint16_t)0;

  uint8_t ReadBuff[BUFFPIXEL_X3(BuffPixel)];
  //clear buffer
  for (uint16_t N = 0; N < BUFFPIXEL_X3(BuffPixel); N++) {
    ReadBuff[N] = 0;
  }






  LCD_SetWindow(0, 0, 480, 320);

  LCD_DC_1;
  LCD_CS_0;

  for(int y = 0; y < 320; y++){
    for(int x = 0; x < 480; x++){
      // myFile.read(ReadBuff, BUFFPIXEL_X3(BuffPixel));

      // Serial.println((int)ReadBuff[0]);
      // Serial.println((int)ReadBuff[1]);
      // Serial.println((int)ReadBuff[2]);

      // DrawPixel(
      //   x, y,
      //   RGB24TORGB16( ReadBuff[2], ReadBuff[1], ReadBuff[0])
      // );

      int x2 = x % 128;
      int y2 = y % 128;

      unsigned char r = clampChannel(255 - 2*x2);
      unsigned char g = clampChannel(2*y2);

      // Serial.print("X: ");
      // Serial.print(x);
      // Serial.print("; Y: ");
      // Serial.print(y);
      // Serial.print("; R: ");
      // Serial.print((unsigned int)r);
      // Serial.print("; G: ");
      // Serial.print((unsigned int)g);
      // Serial.print("\n");

      COLOR color = RGB24TORGB16(r, g, 0);

      SPI4W_Write_Byte(color >> 8);
      SPI4W_Write_Byte(color & 0XFF);
      
    }
  }

  LCD_CS_1;

  // Close the file
  myFile.close();








  // for ( Y_Point = 0; Y_Point < BMP_Dis.BMP_Height; Y_Point++) {//Total display column
  //   for ( X_Point = 0; X_Point < BMP_Dis.BMP_Width / BuffPixel; X_Point++) {//Show a line in the line
      
  //     }

  //     // //show bmp to LCD
  //     // int k = 0;
  //     // LCD_SetWindow( X_Point * BuffPixel , Y_Point, ( X_Point + 1 ) * BuffPixel , Y_Point);
  //     // LCD_DC_1;
  //     // LCD_CS_0;
  //     // for (uint16_t j = 0; j < BuffPixel; j++) {
  //     //   k = j * 3;
  //     //   SPI4W_Write_Byte( (RGB24TORGB16( ReadBuff[k + 2], ReadBuff[k + 1], ReadBuff[k])) >> 8);
  //     //   SPI4W_Write_Byte( (RGB24TORGB16(ReadBuff[k + 2], ReadBuff[k + 1], ReadBuff[k]))  & 0XFF);
  //     // }
  //     // LCD_CS_1;
  //   }
  // }

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
