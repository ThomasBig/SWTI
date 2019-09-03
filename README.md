# SWTI
#### Simple Windows Text Interface
This library wraps Windows API to create library easy to use.
It is in C++ code.
It has all the important function made for you.
It is fully customizable.

The SWTI Library is made from 4 objects
* **Cursor** prints colorful text on screen
* **Keyboard** checks keyboard input
* **Mouse** detects mouse position on screen
* **Window** used for resizing console window

For further information, check the [WIKI](https://github.com/ThomasBig/SWTI/wiki).

## Instalation
Installation of SWTI library is easy.
Download the repository and copy the folder *SWTI/swti* to your project folder.
It contains the header and source code for the library.
You can now use the SWTI library as any other library.

### Hello world
You can use this simple example to ensure that SWTI library is installed correctly. Following code writes colorful Hello world on a specified position. It uses the Cursor object and its methods setPosition() and setColor().

```c++
#include <iostream>
#include "swti/swti.h"

int main()
{
  Cursor.setPosition(5,2);
  Cursor.setColor(YELLOW);
  std::cout << "Hello world!" << std::endl;
}
```

### Creating games
With this library making games is easy and fun.
Its great for beginner and advanced programmers.
You don't have to learn a whole new language or graphical library.
You use standard output to print on screen.

![platformer game](https://i.imgur.com/lF9FA1C.gif)

**platformer game**

### Improved aplications
![custom application](https://i.imgur.com/I4jxzWY.gif)


## Contributing
Larger projects often have sections on contributing to their project, in which contribution instructions are outlined. Sometimes, this is a separate file. If you have specific contribution preferences, explain them so that other developers know how to best contribute to your work. To learn more about how to help others contribute, check out the guide for setting guidelines for repository contributors.

## Credits
This library is currently made by one person. Feel free to **contribute**.

## License
The library is licensed by MIT License. You can use the
