# SWTI
#### Simple Windows Text Interface
This library wraps Windows API to create library easy to use.
This library M.
It has all the important function made for you.
It is fully customizable.

The SWTI Library is made from 4 objects
* **Cursor** prints colorful text on screen
* **Keyboard** checks keyboard input
* **Mouse** detects mouse position on screen
* **Window** used for resizing console window

For further information, check the [WIKI](https://github.com/ThomasBig/SWTI/wiki).

## Hello world
Cursor writes in console window all text. Most important functions are SetPosition, SetColor, clearScreen. Following example writes colorful *Hello world* on screen on specified position.
```c++
#include <iostream>
#include "swti/swti.cpp"

int main()
{
  Cursor.setPosition(5,2);
  Cursor.setColor(YELLOW);
  std::cout << "Hello world!" << std::endl;
}
```

## Awesome games
With this library making games is easy and fun.
Its great for beginner and advanced programmers.
You don't have to learn a whole new language or graphical library.
You use standard output to print on screen.

![platformer game](https://i.imgur.com/lF9FA1C.gif)
**platformer game**

## Beutiful aplications
![custom application](https://i.imgur.com/I4jxzWY.gif)


## Contributing
Larger projects often have sections on contributing to their project, in which contribution instructions are outlined. Sometimes, this is a separate file. If you have specific contribution preferences, explain them so that other developers know how to best contribute to your work. To learn more about how to help others contribute, check out the guide for setting guidelines for repository contributors.

## Credits
Include a section for credits in order to highlight and link to the authors of your project.

## License
Finally, include a section for the license of your project. For more information on choosing a license, check out GitHub’s licensing guide!

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
