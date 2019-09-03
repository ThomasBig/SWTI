#include <iostream>
#include "swti/swti.cpp"

const int pixel = 219;
const Color colors[] = {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, LIGHTGRAY,  DARKGRAY,
  LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, LIGHTYELLOW, WHITE};

int main()
{
  Cursor.setFontPixels(100,100);
  for (int i = 0; i < 16; i++)
  {
    Cursor.printChar(i, 0, pixel, colors[i]);
  }
}
