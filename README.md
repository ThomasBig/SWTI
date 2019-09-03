# SWTI
#### Simple Windows Text Interface
This library wraps Windows API to create library easy to use.
This library M.
It has all the important function made for you.
It is fully customizable.

The SWTI Library is made from 4 objects
* [**Cursor**](docs\module\cursor.md) prints colorful text on screen
* [**Keyboard**](docs\module\keyboard.md) checks keyboard input
* [**Mouse**](docs\module\mouse.md) detects mouse position on screen
* [**Window**](docs\module\window.md) used for resizing console window

For further information, check the [WIKI](https://thomasbig.github.io/SWTI/).

## Awesome games
With this library making games is easy and fun.
Its great for beginner and advanced programmers.
You don't have to learn a whole new language or graphical library.
You use standard output to print on screen.

![platformer game](https://i.imgur.com/lF9FA1C.gif)
**platformer game**

## Beutiful aplications
![custom application](https://i.imgur.com/I4jxzWY.gif)

## Quick setup

```c++
#include <iostream>
#include "swti/swti.cpp"

int main()
{
  Cursor.hideBlinking();
  Cursor.hideSelection();

  while(!Keyboard.get(VK_ESCAPE))
  {
    if (Mouse.getRows() == 2) Cursor.setColor(LIGHTGREEN);
    else Cursor.setColor(YELLOW);

    Cursor.setPosition(5,2);
    std::cout << "Start the application";
    Keyboard.wait(30);
  }
}
```
