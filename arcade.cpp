#include <iostream>
#include <vector>
#include <time.h>
#include "swti/swti.hpp"

////////////////////////////////////////////////////////////////
//                     ARCADE SWTI GAME                       //
//                   controls - W A S D                       //
//                 player, enemies and walls                  //
////////////////////////////////////////////////////////////////

// Set global difficulty
enum difs {EASY = 1, MEDIUM = 2, HARD = 3};
const int DIFFICULTY = MEDIUM;

// Make generic object, used for walls, enemies and player
class Object
{
protected:
  int x, y; // position in collumns and rows
  char skin; // printed character
  Color color; // color of the object

public:
  Object(int xpos, int ypos, char type, Color col) :
    x(xpos), y(ypos), skin(type), color(col) { show(); }
  Object(int xpos, int ypos) : Object(xpos, ypos, FILL_F, GRAY)  { }
  Object() : Object(0, 0) { }

  void show()
    { Cursor.printChar(x, y, skin, color); }
  void hide()
    { Cursor.printBlank(x, y); }
  bool collision(int xt, int yt)
    { return x == xt && y == yt; }
};

bool wallCollision(int x, int y, std::vector<Object*>& walls)
{
  for(auto& w: walls)
    if (w -> collision(x, y))
      return true;
  return false;
}

class Player : public Object
{
public:
  Player(int px, int py) : Object(px, py, '*', LIGHTGREEN) { }
  Player() : Player (1, 1) { }
  int getX() { return x; }
  int getY() { return y; }

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

class Enemy : public Object
{
protected:
  int cooldown;

public:
  Enemy(int px, int py) : Object(px, py, 158, LIGHTRED) { cooldown = 0; }
  Enemy() : Enemy (1, 1) { }

  void move(Player player, std::vector<Object*>& walls)
  {
    if (DIFFICULTY + cooldown++ > 3)
    {
      int nx = x, ny = y;
      if (y > player.getY() && !wallCollision(x,y-1,walls)) ny--;
      else if (y < player.getY() && !wallCollision(x,y+1,walls)) ny++;
      else if (x > player.getX() && !wallCollision(x-1,y,walls)) nx--;
      else if (x < player.getX() && !wallCollision(x+1,y,walls)) nx++;
      if ((nx != x || ny != y) && !wallCollision(nx, ny, walls))
        { hide(); x = nx; y = ny; show(); }
      cooldown = 0;
    }
  }
};

void create_walls(std::vector<Object*>& walls)
{
  int posx, posy;
  for (int i = 0; i < 18; i++)
  {
    walls.push_back(new Object(i,0));
    walls.push_back(new Object(i+1,18));
    walls.push_back(new Object(0,i+1));
    walls.push_back(new Object(18,i));
  }

  for (int i = 0; i < 100; i++)
  {
    posx = 2 + rand() % 15;
    posy = 2 + rand() % 15;
    if (posx != 1 && posy != 1)
      walls.push_back(new Object(posx, posy));
  }
}

void create_enemies(std::vector<Enemy*>& enemies, std::vector<Object*>& walls)
{
  int posx, posy;
  for (int i = 0; i < DIFFICULTY + rand() % 3; i++)
  {
    do {
      posx = 2 + rand() % 15;
      posy = 2 + rand() % 15;
    } while (wallCollision(posx, posy, walls));
    enemies.push_back(new Enemy(posx, posy));
  }
}

Object* create_treasure(std::vector<Object*>& walls)
{
  int posx, posy;
  do {
    posx = 8 + rand() % 5;
    posy = 8 + rand() % 5;
  } while (wallCollision(posx, posy, walls));
  return new Object(posx, posy, 'o', LIGHTYELLOW);
}

void end(std::string text, Color col, Player *player,
  std::vector<Object*>& walls, std::vector<Enemy*>& enemies, Object* treasure)
{
  player->show();
  Cursor.setPosition(text.length() > 15 ? 1 : 3, 5);
  Cursor.setColor(col);
  std::cout << text;

  Keyboard.waitUser();
  Cursor.clearScreen();
  walls.clear();
  enemies.clear();
  delete player;
  delete treasure;
}

void game()
{
  Player *player = new Player(1,1);
  std::vector<Object*> walls;
  std::vector<Enemy*> enemies;
  int posx, posy;

  srand(time(NULL));
  create_walls(walls);
  create_enemies(enemies, walls);
  Object* treasure = create_treasure(walls);

  Keyboard.waitUser();
  while(!Keyboard.get(VK_ESCAPE))
  {
    player -> move(walls);
    if (treasure -> collision(player -> getX(), player -> getY()))
    {
      end("You got treasure!", LIGHTYELLOW, player, walls, enemies, treasure);
      return game();
    }

    for(auto& enemy: enemies)
    {
      enemy -> move(*player, walls);
      if (enemy -> collision(player -> getX(), player -> getY()))
      {
        end("You are dead!", LIGHTRED, player, walls, enemies, treasure);
        return game();
      }
    }

    treasure -> show();
    Keyboard.wait(30);
  }
}

int main()
{
  int fontSize = Window.getScreenHeight() / 40;
  Cursor.setFontPixels(fontSize, fontSize);
  Window.setSizeChars(20, 20);
  Window.hideResize();
  Window.hideBlinking();
  Window.hideScrollbars();
  Window.setTitle("Arcade example");
  game();
  return 0;
}
