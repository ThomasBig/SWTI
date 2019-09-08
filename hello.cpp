#include <iostream>
#include "swti/swti.h"

using namespace std;

// simple function to print colorful logo
void printLogo(int x, int y)
{
  Cursor.printChar(x++,y,'S',YELLOW);
  Cursor.printChar(x++,y,'W',LIGHTRED);
  Cursor.printChar(x++,y,'T',LIGHTBLUE);
  Cursor.printChar(x++,y,'I',LIGHTGREEN);
}

int main()
{
  // create event
  int middle, center;
  Cursor.setFontSize(80);
  Window.setFullscreenBorderless();
  middle = Window.getColumns()/2 - 8;
  center = Window.getRows()/2 - 2;

  // main event
  printLogo(middle, center);
  Cursor.setPosition(middle + 5, center);
  Cursor.setColor(YELLOW);
  cout << "Hello world!";

  // end event
  Cursor.setPosition(0, 0);
  Cursor.setColor(WHITE);
  return 0;
}
