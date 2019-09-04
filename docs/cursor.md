# Cursor

Cursor writes in console window all text. You can change its position and color.

It can change color to one of the following 16 different colors.
![cursor colors](https://i.imgur.com/WDbGTLZ.png)

Following example writes colorful *Hello world* on screen on specified position.
```c++
Cursor.setPosition(5,2);
Cursor.setColor(YELLOW);
std::cout << "Hello world!" << std::endl;
```

![Cursor showcase](https://i.imgur.com/yPnCwQG.gif)
