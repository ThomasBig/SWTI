#include <iostream>
#include "swti/swti.cpp"

int main()
{
  Window.hideBlinking();
  Window.hideSelection();

  while(!Keyboard.get(VK_ESCAPE))
  {
    if (Mouse.getRows() == 2) Cursor.setColor(LIGHTGREEN);
    else Cursor.setColor(YELLOW);

    Cursor.setPosition(5,2);
    std::cout << "Start the application";
    Keyboard.wait(30);
  }
}
