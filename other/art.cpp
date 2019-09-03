#include <iostream>
#include "swti/swti.cpp"

using namespace std;

int main()
{
  int mx, my;
  Cursor.hideBlinking();
  Cursor.hideScrollbars();
  Cursor.hideSelection();

  while(!Keyboard.get(VK_ESCAPE)) // escape key isn't pressed
  {
    mx = Mouse.getColumns();
    my = Mouse.getRows();
    if (Keyboard.get(VK_LBUTTON))
      if (mx > 0 && my > 0 && mx < Window.getColumns() && my < Window.getRows())
        Cursor.printChar(mx,my,'o',LIGHTBLUE);
  }
}
