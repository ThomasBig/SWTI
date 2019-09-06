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

bool wallCollision(int x, int y, std::vector<Object*>& walls)
{
  bool col = false;

  for(auto& w: walls)
  {
    if (w->collision(x,y))
    {
      return true;
    }
  }

  return false;
}

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
    if (Keyboard.get('W') && !wallCollision(x,y-1,walls))
    {
      ny--;
    }
    if (Keyboard.get('S') && !wallCollision(x,y+1,walls))
    {
      ny++;
    }
    if (Keyboard.get('A') && !wallCollision(x-1,y,walls))
    {
      nx--;
    }
    if (Keyboard.get('D') && !wallCollision(x+1,y,walls))
    {
      nx++;
    }
    if (nx != x || ny != y)
    {
      hide();
      x = nx;
      y = ny;
      show();
    }
  }
};



int main()
{
  Cursor.setFontSize(40);
  Window.hideBlinking();
  Window.setTitle("Arcade example");

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
  //walls.push_back(new Object(6,4));
  //walls.push_back(new Object(5,5));
  walls.push_back(new Object(6,5));

  while(!Keyboard.get(VK_ESCAPE))
  {
    player.move(walls);
    Keyboard.wait(30);
  }

  return 0;
}
