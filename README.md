# SWTI
#### Simple Windows Text Interface
This library wraps Windows API to create a library easy to use.
It is build in C++ code for Windows operating system.
It is great for building any text based application or game.
You can watch a [showcase](youtube.com) video to see applications made with a help of SWTI.

The SWTI Library is made from 4 objects.
* **Cursor** prints colorful text on screen
* **Keyboard** checks keyboard input
* **Mouse** detects mouse position on screen
* **Window** sets the console window size

For further information, check out the [WIKI](https://github.com/ThomasBig/SWTI/wiki).


## Instalation
The installation of SWTI library is easy.
Download the repository and copy the folder *SWTI/swti* to your project folder.
It contains header and source code for the library.
In your application include a swti header or a source file.

### Hello world
You can use this simple example to ensure that SWTI library is installed correctly. Following code writes colorful Hello world on a specified position. It uses the Cursor object and methods setColor and setPosition.

```c++
#include <iostream>
#include "swti/swti.h"

int main()
{
  Cursor.setColor(YELLOW);
  Cursor.setPosition(5,2);
  std::cout << "Hello world!" << std::endl;
}
```


### Creating Aplications
You can use the library to create fast and nice looking text interface.
![custom application](https://i.imgur.com/I4jxzWY.gif)


### Making Games
Making games with the library is easy and fun.
It is great start for beginner programmers.

![platformer game](https://i.imgur.com/lF9FA1C.gif)


## Contributing
You can help me by contributing to the library.
If you find a bug, please write it in [issues](https://github.com/ThomasBig/SWTI/issues).
If you write a code for a new useful function you can add a merge [request](https://github.com/ThomasBig/SWTI/pulls).
Keep in mind the new function has to be usable in any applications and game. 
Excluding specific functions makes library simple to use.


## Credits
This library is currently maintained by one person. Feel free to **contribute**.


## License
The library is licensed by MIT License. You can use the code in any application, but without warranty. That means you can make any commercial application with the library but I am not responsible for any crashes and further problems.
