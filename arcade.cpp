#include <iostream>
#include <vector>
#include "swti/swti.h"

class Object
{
protected:
  int x, y;
  virtual void show() { Cursor.printChar(x, y, 219, GRAY); }
  virtual void hide() { Cursor.printBlank(x, y); }

public:
  Object() { x = 2; y = 2; }
  Object(int xpos, int ypos) { x = xpos; y = ypos; show(); }
  bool collision(int xt, int yt) { return (x == xt) && (y == yt); }
};

bool wallCollision(int x, int y, std::vector<Object*>& walls)
{
  for(auto& w: walls) if (w->collision(x, y)) return true;
  return false;
}

class Player : Object
{
protected:
  void show() { Cursor.printChar(x, y, '*', GREEN); }

public:
  int getX() { return x; }
  int getY() { return y; }
  Player() { x = 1; y = 1; show(); }
  Player(int px, int py) { x = px; y = py; show(); }
  void move(std::vector<Object*>& walls)
  {
    int nx = x, ny = y;
    if (Keyboard.get('W') && !wallCollision(x, y-1, walls)) ny--;
    if (Keyboard.get('S') && !wallCollision(x, y+1, walls)) ny++;
    if (Keyboard.get('A') && !wallCollision(x-1, y, walls)) nx--;
    if (Keyboard.get('D') && !wallCollision(x+1, y, walls)) nx++;
    if ((nx != x || ny != y) && !wallCollision(nx, ny, walls))
      { hide(); x = nx; y = ny; show(); }
  }
};

class Enemy : Object
{
protected:
  void show() { Cursor.printChar(x, y, 158, RED); }
  int cooldown;
public:
  Enemy() {x = 1; y = 1; cooldown = 0; show();}
  Enemy(int px, int py) { x = px; y = py; cooldown = 0; show(); }

  void move(Player player, std::vector<Object*>& walls)
  {
    if (cooldown++ > 1)
    {
      int nx = x, ny = y;
      if (y > player.getY() && !wallCollision(x,y-1,walls)) ny--;
      if (y < player.getY() && !wallCollision(x,y+1,walls)) ny++;
      if (x > player.getX() && !wallCollision(x-1,y,walls)) nx--;
      if (x < player.getX() && !wallCollision(x+1,y,walls)) nx++;
      if ((nx != x || ny != y) && !wallCollision(nx, ny, walls))
        { hide(); x = nx; y = ny; show(); }
      cooldown = 0;
    }
  }
};


int main()
{
  Cursor.setFontPixels(20,20);
  Window.hideBlinking();
  Window.setTitle("Arcade example");

  Player *player = new Player(1,1);
  std::vector<Object*> walls;
  std::vector<Enemy*> enemies;

  for (int i = 0; i < 10; i++)
  {
    walls.push_back(new Object(i,0));
    walls.push_back(new Object(i+1,10));
    walls.push_back(new Object(0,i+1));
    walls.push_back(new Object(10,i));
  }

  walls.push_back(new Object(5,1));
  walls.push_back(new Object(1,5));

  walls.push_back(new Object(4,5));
  walls.push_back(new Object(5,4));
  walls.push_back(new Object(6,5));

  enemies.push_back(new Enemy(7, 1));
  enemies.push_back(new Enemy(1, 7));

  while(!Keyboard.get(VK_ESCAPE))
  {
    player->move(walls);
    for(auto& e: enemies) e->move(*player,walls);
    Keyboard.wait(30);
  }

  return 0;
}
