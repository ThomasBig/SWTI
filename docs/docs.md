# SWTI
#### Simple Windows Text Interface
This library wraps Windows API to create easy to use library.
It has all the important function made for you.

The SWTI Library is created with 4 objects
* [**Cursor**](docs\module\cursor.md) prints colorful text on screen
* [**Keyboard**](docs\module\keyboard.md) checks keyboard input
* [**Mouse**](docs\module\mouse.md) detects mouse position on screen
* [**Window**](docs\module\window.md) used for resizing console window

## Start simple
Cursor writes in console window all text. Most important functions are SetPosition, SetColor, clearScreen. Following example writes colorful *Hello world* on screen on specified position.
```c++
Cursor.setPosition(5,2);
Cursor.setColor(YELLOW);
std::cout << "Hello world!" << std::endl;
```
