#include "MiniGraphics.h"
#include "../GraphicsDriver/LCD_GUI.h"

void DrawChar(uint16_t Xpoint, uint16_t Ypoint, const char AsciiChar, sFONT* Font, COLOR color){
  uint32_t Char_Offset = (AsciiChar - ' ') * Font->Height * (Font->Width / 8 + (Font->Width % 8 ? 1 : 0));
  const unsigned char *ptr = &Font->table[Char_Offset];

  for (uint16_t Page  = 0; Page < Font->Height; Page ++ ) {
    for (uint16_t Column = 0; Column < Font->Width; Column ++ ) {
        // 10000000 >> xOffset
        if (pgm_read_byte(ptr) & (0x80 >> (Column % 8))) {
          GUI_DrawPoint(Xpoint + Column, Ypoint + Page, color, DOT_PIXEL_DFT, DOT_STYLE_DFT);
        }
        if (Column % 8 == 7) {
          ptr++;
        }
    }
    if (Font->Width % 8 != 0) {
      ptr++;
    }
  }
}
