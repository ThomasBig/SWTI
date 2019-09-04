# Welcome to SWTI Docs

The SWTI Library is made out of 4 parts

* [Cursor](https://thomasbig.github.io/SWTI/cursor/) which is used for printing colorful text on screen

* [Keyboard](https://thomasbig.github.io/SWTI/keyboard/) which is used for detecting user keyboard input

* [Mouse](https://thomasbig.github.io/SWTI/mouse/) which is used for detecting user mouse input

* [Window](https://thomasbig.github.io/SWTI/window/) which is used for resizing window

These four objects have in total about 40 methods to interact with them.

## Instalation
The installation of SWTI library is easy.
Download the github [repository](https://github.com/ThomasBig/SWTI) and copy the folder *SWTI/swti* to your project folder.
It contains header and source code for the library.
In your application include a swti header and a source file.


## Start example
You can use folowing code to ensure that all the objects are working.
This code writes on screen a yellow text and when a colors it when mouse hovers over it.
``` c++
#include <iostream>
#include "swti/swti.h"

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
```
