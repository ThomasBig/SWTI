#include <iostream>
#include <vector>
#include "swti/swti.cpp"

class Object
{
protected:
  int x, y;
public:
  virtual void show()
  {
    Cursor.printChar(x, y,'#', WHITE);
  }
  void hide()
  {
    Cursor.printBlank(x, y);
  }
  bool collision(int xt, int yt)
  {
    return (x == xt) && (y == yt);
  }
  Object() {x = 2; y = 2;}
  Object(int xpos, int ypos)
    {x = xpos; y = ypos;show();}
};

class Player : Object
{
public:
  void show()
  {
    Cursor.printChar(x, y,'O', GREEN);
  }
  Player(){x = 1; y = 1;show();}
  void move(std::vector<Object*>& walls)
  {
    int nx = x, ny = y;
    if (Keyboard.get('W'))
    {
      ny = y-1;
    }
    if (Keyboard.get('S'))
    {
      ny = y+1;
    }
    if (Keyboard.get('A'))
    {
      nx = x-1;
    }
    if (Keyboard.get('D'))
    {
      nx = x+1;
    }
    if (nx != x || ny != y)
    {
      bool col = false;
      for(auto& w: walls)
      {
        if (w->collision(nx,ny))
        {
          col = true;
          break;
        }
      }

      if (!col)
      {
        hide();
        x = nx;
        y = ny;
        show();
      }
    }
  }
};

int main()
{
  Cursor.setFontSize(40);
  Cursor.hideBlinking();
  Cursor.setFontSize(40);
  Window.setTitle("Arcade example");
  Window.setFullscreenWindow();

  Player player;
  std::vector<Object*> walls;

  for (int i = 0; i < 10; i++)
  {
    walls.push_back(new Object(i,0));
    walls.push_back(new Object(i+1,10));
    walls.push_back(new Object(0,i+1));
    walls.push_back(new Object(10,i));
  }

  walls.push_back(new Object(5,4));
  walls.push_back(new Object(6,4));
  walls.push_back(new Object(5,5));
  walls.push_back(new Object(6,5));

  while(!Keyboard.get(VK_ESCAPE))
  {
    player.move(walls);
    Keyboard.wait(30);
  }

  return 0;
}
