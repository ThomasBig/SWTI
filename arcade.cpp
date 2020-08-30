#include <iostream>
#include <vector>
#include <time.h>
#include "swti/swti.hpp"

////////////////////////////////////////////////////////////////
//                      ARCADE SWTI GAME                      //
//                     controls - W A S D                     //
//                 player, enemies and walls                  //
////////////////////////////////////////////////////////////////

// Set global difficulty
enum difficulties { EASY = 1, MEDIUM = 2, HARD = 3 };
const int DIFFICULTY = MEDIUM;

// General class, used for walls, enemies and player
class Object
{
protected:
  int x, y; // position in collumns and rows
  char skin; // printed character
  Color color; // color of the object

public:
  // generic constructors that will assign all the variables
  Object(int xpos, int ypos, char type, Color col) :
    x(xpos), y(ypos), skin(type), color(col) { show(); }
  // constructor for walls
  Object(int xpos, int ypos) : Object(xpos, ypos, (char) FILL_F, GRAY)  { }
  Object() : Object(0, 0) { }

  void show() // show object in console
    { Cursor.printChar(x, y, skin, color); }

  void hide() // hide object in console
    { Cursor.printBlank(x, y); }

  // check collision on my position
  bool collision(int xt, int yt)
    { return x == xt && y == yt; }
};

// function that will check collision with any object in vector
bool wallCollision(int x, int y, std::vector<Object*>& walls)
{
  // loop through all the walls
  for(auto wall: walls)
    if (wall -> collision(x, y))
      return true;
  return false;
}

// Class for player, based on object but add movement
class Player : public Object
{
public:
  // Player is light green star
  Player(int px, int py) : Object(px, py, '*', LIGHTGREEN) { }
  Player() : Player (1, 1) { }

  // Get position for enemy class
  int getX() { return x; }
  int getY() { return y; }

  // Move player with WASD keys, check for wall collision
  void move(std::vector<Object*>& walls)
  {
    int nx = x, ny = y; // reprint player only if position was changed
    if (Keyboard.get('W') && !wallCollision(x, y-1, walls)) { ny--; }
    if (Keyboard.get('S') && !wallCollision(x, y+1, walls)) { ny++; }
    if (Keyboard.get('A') && !wallCollision(x-1, y, walls)) { nx--; }
    if (Keyboard.get('D') && !wallCollision(x+1, y, walls)) { nx++; }
    if ((nx != x || ny != y) && !wallCollision(nx, ny, walls))
      { hide(); x = nx; y = ny; show(); }
  }
};

// Enemy class, based on object but with automatic movement
class Enemy : public Object
{
public:
  // Enemy is red X character
  Enemy(int px, int py) : Object(px, py, (char) 158, LIGHTRED) { }
  Enemy() : Enemy (1, 1) { }

  // Move to player based on their location
  void move(Player* player, Object* treasure, std::vector<Object*>& walls)
  {
    int nx = x, ny = y; // redraw enemy only if neccessary
    if (y > player->getY() && !wallCollision(x, y-1, walls)) { ny--; }
    else if (y < player->getY() && !wallCollision(x, y+1, walls)) { ny++; }
    else if (x > player->getX() && !wallCollision(x-1, y, walls)) { nx--; }
    else if (x < player->getX() && !wallCollision(x+1, y, walls)) { nx++; }
    if ((nx != x || ny != y) && !wallCollision(nx, ny, walls))
    {
      if (!treasure->collision(x, y))
        { hide(); }
      else
        { treasure->show(); }
      x = nx;
      y = ny;
      show();
    }
  }
};

// create wall border and random level within
void create_walls(std::vector<Object*>& walls)
{
  // border
  int posx, posy;
  for (int i = 0; i < 18; i++)
  {
    walls.push_back(new Object(i,0));
    walls.push_back(new Object(i+1,18));
    walls.push_back(new Object(0,i+1));
    walls.push_back(new Object(18,i));
  }

  // random walls inside the border
  for (int i = 0; i < 100; i++)
  {
    posx = 2 + rand() % 15;
    posy = 2 + rand() % 15;
    if (posx != 1 && posy != 1 && !wallCollision(posx, posy, walls))
      { walls.push_back(new Object(posx, posy)); }
  }
}

// create random number of enemies on random positions
// cannot create enemy in wall but can create enemy inside treasure object
void create_enemies(std::vector<Enemy*>& enemies, std::vector<Object*>& walls)
{
  int posx, posy;
  // create enemies based on difficulty
  for (int i = 0; i < DIFFICULTY + rand() % 3; i++)
  {
    // can't create enemies on wall position
    do {
      posx = 2 + rand() % 15;
      posy = 2 + rand() % 15;
    } while (wallCollision(posx, posy, walls));
    enemies.push_back(new Enemy(posx, posy));
  }
}

// spawn a random treasure inside the level
// sometimes treasure will be unreachable
Object* create_treasure(std::vector<Object*>& walls)
{
  // check collision with walls
  int posx, posy;
  do {
    posx = 8 + rand() % 5;
    posy = 8 + rand() % 5;
  } while (wallCollision(posx, posy, walls));
  return new Object(posx, posy, 'o', LIGHTYELLOW);
}

// end the level with custom message, free allocated memory
void end(std::string text, Color col, Player *player,
  std::vector<Object*>& walls, std::vector<Enemy*>& enemies, Object* treasure)
{
  player->show();
  Cursor.setPosition(text.length() > 15 ? 1 : 3, 5);
  Cursor.setColor(col);
  std::cout << text;

  // clear everything after user presses a button
  Keyboard.waitUser();
  Cursor.clearScreen();
  walls.clear();
  enemies.clear();
  delete player;
  delete treasure;
}

// play the game
bool game()
{
  // create storage for objects
  Player *player = new Player(1,1);
  std::vector<Object*> walls;
  std::vector<Enemy*> enemies;
  int cooldown = 0;

  // create random environment
  srand((int)time(NULL));
  create_walls(walls);
  create_enemies(enemies, walls);
  Object* treasure = create_treasure(walls);

  // wait for first keyboard interaction
  Keyboard.waitUser();

  // play one game
  while(!Keyboard.get(VK_ESCAPE))
  {
    // move player and check collision with treasure
    player -> move(walls);
    if (treasure -> collision(player -> getX(), player -> getY()))
    {
      end("You got treasure!", LIGHTYELLOW, player, walls, enemies, treasure);
      return true;
    }

    // loop through enemies and move them
    for(auto& enemy: enemies)
    {
      // Move only if cooldown has expired
      if (DIFFICULTY + cooldown > 3)
      {
        enemy -> move(player, treasure, walls);
      }
      if (enemy -> collision(player -> getX(), player -> getY()))
      {
        end("You are dead!", LIGHTRED, player, walls, enemies, treasure);
        return true;
      }
    }
    if (DIFFICULTY + cooldown++ > 3)
    {
      cooldown = 0;
    }
    // wait 30 ticks per second
    Keyboard.wait(30);
  }

  end("See you soon.", LIGHTYELLOW, player, walls, enemies, treasure);
  return false;
}

// configurate window and start the game
int main()
{
  int fontSize = Window.getScreenHeight() / 36;
  Cursor.setFontPixels(fontSize, fontSize);
  Window.setSizeChars(20, 20);
  Window.hideResize();
  Window.hideBlinking();
  Window.hideScrollbars();
  Window.setTitle(L"Arcade example");

  // play multiple games
  while(game()) { }
  return 0;
}
