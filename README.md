# SWTI
#### Simple Windows Text Interface
This library wraps Windows API to create a library easy to use.
It is build in C++ code for Windows operating system.
It is great for building any text based application or game.
You can watch a [showcase video](https://youtu.be/V11sTuCHC_A).

The SWTI Library is made from 4 objects.
* **Cursor** prints colorful text on screen
* **Keyboard** checks keyboard input
* **Mouse** detects mouse position on screen
* **Window** sets the console window size

For further information, check out the [documentation](https://thomasbig.github.io/SWTI/) (in development).


### Creating Aplications
You can use the library to create fast and nice looking text interface.

![custom application](https://i.imgur.com/I4jxzWY.gif)


### Making Games
Making games with the library is easy and fun.
It is great start for beginner programmers.

![platformer game](https://i.imgur.com/lF9FA1C.gif)

## Instalation
The installation of SWTI library is easy.
Download this repository and copy the folder *SWTI/swti* to your project folder.
It contains header and source code for the library.
In your application, include a header `"#include swti/swti.h"`.

* **Visual Studio** - you will need to add header and source files to your project. Right click on the right folder *header files* and add *existing file* and choose *SWTI/swti.h*. Do the same for *source files* and add *SWTI/swti.cpp*.

* **Code Blocks** - click on the left pane folders and add existing header and source files to your project.

* **G++ Console** - You can then use following two commands `g++ -c main.cpp swti/swti.cpp` and `g++ -o main.exe main.o swti.o` to compile your project. Change *main.cpp* and *main.exe* to whatever you prefer.


### Hello world
You can use this simple example to ensure that SWTI library is installed correctly.
Following code writes colorful Hello world on a specified position.
It uses the Cursor object and methods setColor and setPosition.

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


## Contributing
You can help me by contributing to the library.
If you find a bug, please write it in [issues](https://github.com/ThomasBig/SWTI/issues). If you write a code for a new useful function you can add a merge [request](https://github.com/ThomasBig/SWTI/pulls).
Keep in mind the new function has to be usable in any applications and game. Excluding too specific functions makes library simple to learn and easy to use.


## Credits
This library is currently maintained by one person. Feel free to [contribute](https://thomasbig.github.io/SWTI/about/#contributing).


## License
The library is licensed under [MIT License](https://github.com/ThomasBig/SWTI/blob/master/LICENSE.txt). You can use the code in any application, but without warranty. That means you can make any application with the library but I am not responsible for any crashes and further problems.
