# Cursor
Cursor is used to prints text on screen. You can change the

It can change color to one of the following 16 different colors.
![cursor colors](https://i.imgur.com/WDbGTLZ.png)

Cursor writes in console window all text. Most important functions are SetPosition, SetColor, clearScreen. Following example writes colorful *Hello world* on screen on specified position.
```c++
Cursor.setPosition(5,2);
Cursor.setColor(YELLOW);
std::cout << "Hello world!" << std::endl;
```
