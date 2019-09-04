# Welcome to SWTI Docs

The SWTI Library is made out of 4 parts

* [Cursor](https://github.com/ThomasBig/SWTI/wiki/Cursor) which is used for printing colorful text on screen

* [Keyboard](https://github.com/ThomasBig/SWTI/wiki/Keyboard) which is used for detecting user keyboard input

* [Mouse](https://github.com/ThomasBig/SWTI/wiki/Mouse) which is used for detecting user mouse input

* [Window](https://github.com/ThomasBig/SWTI/wiki/Window) which is used for resizing window

These four objects have in total about 40 methods to interact with them.

## Instalation
The installation of SWTI library is easy.
Download the github [repository](https://github.com/ThomasBig/SWTI) and copy the folder *SWTI/swti* to your project folder.
It contains header and source code for the library.
In your application include a swti header and a source file.


## Start example
This code writes on screen a yellow text and when a colors it when mouse hovers over it.
``` c++
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
```
