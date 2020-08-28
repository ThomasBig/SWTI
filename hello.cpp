#include <iostream>
#include "swti/swti.hpp"

////////////////////////////////////////////////////////////////
//                    SIMPLE SWTI EXAMPLE                     //
//                   fullcreen hello world                    //
////////////////////////////////////////////////////////////////

using namespace std;

// set fullsreen, font size and colors
void setup()
{
  Window.setDefaultColor(BLACK, RGB(16, 20, 28));
  Window.setDefaultColor(YELLOW, HEX(0xf5f118));
  int size = Window.getScreenHeight() / 15;
  Cursor.setFontSize(size);
  Window.setFullscreenBorderless();
  Window.hideBlinking();
}

// simple function to print colorful logo
void printLogo(int x, int y)
{
  Cursor.printChar(x++, y, 'S', YELLOW);
  Cursor.printChar(x++, y, 'W', LIGHTRED);
  Cursor.printChar(x++, y, 'T', LIGHTBLUE);
  Cursor.printChar(x++, y, 'I', LIGHTGREEN);
}

// main function will be called first
int main()
{
  setup();

  int middle, center;
  middle = Window.getColumns() / 2 - 8;
  center = Window.getRows() / 2 - 2;
  printLogo(middle, center);

  Cursor.setPosition(middle + 5, center);
  Cursor.setColor(YELLOW);
  cout << "Hello world!";

  Keyboard.waitUser();
  return 0;
}
