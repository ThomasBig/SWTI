#include <iostream>
#include <sstream>
#include <algorithm> // std::count
#include "swti/swti.h"

enum ASCII {
  //  Graphic characters
  DOTTED_B = 32,  // blank
  DOTTED_L = 176, // low
  DOTTED_M = 177, // medium5
  DOTTED_H = 178, // high
  DOTTED_F = 219, // full

  // Box drawing characters single line
  LINE_H = 196, // horizontal
  LINE_V = 179, // vertical
  LINE_UR = 191, // up right
  LINE_UL = 218, // up left
  LINE_DR = 217, // down right
  LINE_DL = 192, // down left
  LINE_HU = 193, // horizontal up
  LINE_HD = 194, // horizontal down
  LINE_VR = 195, // vertical right
  LINE_VL = 180, // vertical left
  LINE_HV = 197, // horizontal vertical

  // Box drawing characters double line
  DLINE_H = 205, // horizontal
  DLINE_V = 186, // vertical
  DLINE_UR = 187, // up righ
  DLINE_UL = 201, // up left
  DLINE_DR = 188, // down right
  DLINE_DL = 200, // down left
  DLINE_HU = 202, // horizon
  DLINE_HD = 203, // horizontal down
  DLINE_VR = 204, // vertical right
  DLINE_VL = 185, // vertical left
  DLINE_HV = 206, // horizontal vertical
};

enum HAlign {
  LEFT,
  MIDDLE,
  RIGHT
};

enum VAlign {
  TOP,
  CENTER,
  DOWN
};

// fast one line print
void printLine(int x, int y, std::string text)
{
  Cursor.setPosition(x, y);
  std::cout << text;
}

// multi line print, with optional aligns
void printText(int x, int y, std::string text, HAlign halign=LEFT, VAlign valign=TOP)
{
  int xx, yy;
  std::istringstream separate(text);
  std::string line;

  int cnt = std::count(text.begin(), text.end(), '\n');
  switch (valign)
  {
    case TOP:
      yy = 0; break;
    case CENTER:
      yy = cnt/2; break;
    case DOWN:
      yy = cnt; break;
  }

  while(std::getline(separate, line))
  {
    switch (halign)
    {
      case LEFT:
        xx = 0; break;
      case MIDDLE:
        xx = (line.length()-1)/2; break;
      case RIGHT:
        xx = line.length()-1; break;
    }
    printLine(x-xx, (y++)-yy, line);
  }
}

void printFrame(int x, int y, int columns, int rows)
{
  // corners
  Cursor.printChar(x,y,ASCII::LINE_UL);
  Cursor.printChar(x,y+rows+1,ASCII::LINE_DL);
  Cursor.printChar(x+columns+1,y,ASCII::LINE_UR);
  Cursor.printChar(x+columns+1,y+rows+1,ASCII::LINE_DR);

  // horizontal bars
  for (int i = x+1; i <= x+columns; i++)
  {
    Cursor.printChar(i,y,ASCII::LINE_H);
    Cursor.printChar(i,y+rows+1,ASCII::LINE_H);
  }

  //vertical bars
  for (int i = y+1; i <= y+rows; i++)
  {
    Cursor.printChar(x,i,ASCII::LINE_V);
    Cursor.printChar(x+columns+1,i,ASCII::LINE_V);
  }
}

void printFrameDouble(int x, int y, int columns, int rows)
{
  // corners
  Cursor.printChar(x,y,ASCII::DLINE_UL);
  Cursor.printChar(x,y+rows+1,ASCII::DLINE_DL);
  Cursor.printChar(x+columns+1,y,ASCII::DLINE_UR);
  Cursor.printChar(x+columns+1,y+rows+1,ASCII::DLINE_DR);

  // horizontal bars
  for (int i = x+1; i <= x+columns; i++)
  {
    Cursor.printChar(i,y,ASCII::DLINE_H);
    Cursor.printChar(i,y+rows+1,ASCII::DLINE_H);
  }

  //vertical bars
  for (int i = y+1; i <= y+rows; i++)
  {
    Cursor.printChar(x,i,ASCII::DLINE_V);
    Cursor.printChar(x+columns+1,i,ASCII::DLINE_V);
  }
}

void printGrid(int x, int y, int cx, int cy, int sx, int sy)
{
  int rx = x, ry = y;
  for (int py = 0; py < cy; py++, y += sy+1)
  {
    x = rx;
    for (int px = 0; px < cx; px++, x += sx+1)
    {
      Cursor.printChar(x,y,ASCII::LINE_HV); // middle corners

      for (int i = x+1; i <= x+sx; i++) // horizontal bar
        Cursor.printChar(i,y,ASCII::LINE_H);

      for (int i = y+1; i <= y+sy; i++) //vertical bar
        Cursor.printChar(x,i,ASCII::LINE_V);
    }
    Cursor.printChar(rx,y,ASCII::LINE_VR);
    Cursor.printChar(x,y,ASCII::LINE_VL);

    for (int i = y+1; i <= y+sy; i++) // vertical bar
      Cursor.printChar(x,i,ASCII::LINE_V);
  }

  for (int px = 0, x = rx; px < cx; px++, x += sx+1) // last horizontal bars
  {
    Cursor.printChar(x,y,ASCII::LINE_HU); // middle corners
    Cursor.printChar(x,ry,ASCII::LINE_HD);

    for (int i = x+1; i <= x+sx; i++) // horizontal bar
      Cursor.printChar(i,y,ASCII::LINE_H);
  }

  Cursor.printChar(rx,ry,ASCII::LINE_UL); // draw corners
  Cursor.printChar(rx,y,ASCII::LINE_DL);
  Cursor.printChar(x,ry,ASCII::LINE_UR);
  Cursor.printChar(x,y,ASCII::LINE_DR);
}

void printGridInside(int x, int y, int cx, int cy, int sx, int sy)
{
  int rx = x, ry = y;
  for (int py = 0; py < cy; py++, y += sy+1)
  {
    x = rx;
    for (int px = 0; px < cx; px++, x += sx+1)
    {
      Cursor.printChar(x,y,ASCII::LINE_HV); // middle corners
    }
    Cursor.printChar(rx,y,ASCII::LINE_VR);
    Cursor.printChar(x,y,ASCII::LINE_VL);
  }

  for (int px = 0, x = rx; px < cx; px++, x += sx+1) // last horizontal bars
  {
    Cursor.printChar(x,y,ASCII::LINE_HU); // middle corners
    Cursor.printChar(x,ry,ASCII::LINE_HD);
  }

  Cursor.printChar(rx,ry,ASCII::LINE_UL); // draw corners
  Cursor.printChar(rx,y,ASCII::LINE_DL);
  Cursor.printChar(x,ry,ASCII::LINE_UR);
  Cursor.printChar(x,y,ASCII::LINE_DR);
}

void printGridOutside(int x, int y, int cx, int cy, int sx, int sy)
{
  int rx = x;
  for (int py = 0; py < cy; py++, y += sy+1)
  {
    x = rx;
    for (int px = 0; px < cx; px++, x += sx+1)
    {
      for (int i = x+1; i <= x+sx; i++) // horizontal bar
        Cursor.printChar(i,y,ASCII::LINE_H);

      for (int i = y+1; i <= y+sy; i++) // last vertical bar
        Cursor.printChar(x,i,ASCII::LINE_V);
    }
    for (int i = y+1; i <= y+sy; i++) // last vertical bar
      Cursor.printChar(x,i,ASCII::LINE_V);
  }
  for (int px = 0, x = rx; px < cx; px++, x += sx+1)
    for (int i = x+1; i <= x+sx; i++) // last horizontal bar
      Cursor.printChar(i,y,ASCII::LINE_H);
}

int main()
{
  Window.setSizePixels(940,560);
  Cursor.setFontSize(30);
  Window.hideBlinking();
  Window.hideScrollbars();
  Window.hideSelection();
  //Window.setFullscreenBorderless();

  Cursor.setColor(YELLOW);
  printFrameDouble(0,0,35,17);

  Cursor.setColor(WHITE);
  printGrid(2,1,8,8,3,1);

  Cursor.setColor(GREEN);
  printText(30,22,".------.\n|      |\n|      |\n|      |\n|      |\n.------.",MIDDLE,CENTER);
  printLine(30,22,"X");

  Cursor.setColor(WHITE);
  while(!Keyboard.get(VK_ESCAPE)) // escape key isn't pressed
  {
    Cursor.setPosition(5,20);
    std::cout << Mouse.getX() << "  " << Mouse.getY() << "   ";
    Cursor.setPosition(5,21);
    std::cout << Mouse.getColumns() << "   " << Mouse.getRows() << "   ";
    Keyboard.wait(30);
  }
  return 0;
}
