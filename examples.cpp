#include <iostream>
#include <sstream>
#include <algorithm> // std::count
#include "swti/swti.hpp"

// Common ascii characters
enum ASCII {
  //  Graphic characters
  DOTTED_B = 32,  // blank
  DOTTED_L = 176, // low
  DOTTED_M = 177, // medium
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
  DLINE_UR = 187, // up right
  DLINE_UL = 201, // up left
  DLINE_DR = 188, // down right
  DLINE_DL = 200, // down left
  DLINE_HU = 202, // horizon
  DLINE_HD = 203, // horizontal down
  DLINE_VR = 204, // vertical right
  DLINE_VL = 185, // vertical left
  DLINE_HV = 206, // horizontal vertical
};

// Horizontal align for printing multilined text
enum HAlign {
  LEFT,
  MIDDLE,
  RIGHT
};

// Vertical align for printing multilined text
enum VAlign {
  TOP,
  CENTER,
  DOWN
};

// Print one lined text
// Set position and text
void printLine(int x,int y,std::string text)
{
  Cursor.setPosition(x,y);
  std::cout << text;
}

// Print multilined text
// Set position, text and horizontal and vertical align
void printText(int x,int y,std::string text, HAlign halign=LEFT, VAlign valign=TOP)
{
  int xx,yy;
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
    printLine(x-xx,(y++)-yy,line);
  }
}

// Print rectangle using single dashed box drawing characters
// Set position and width and height in columns and rows
void printFrame(int x,int y,int columns, int rows)
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

// Print rectangle using double dashed box drawing characters
// Set position and width and height in columns and rows
void printFrameDouble(int x,int y,int columns, int rows)
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

// Print custom grid using box drawing characters
// Set position, number of columns and rows, size of column and row
void printGrid(int x,int y,int cx,int cy,int sx,int sy)
{
  int px, py, i, rx = x;
  for (py = 0; py < cy; py++, y += sy+1)
  {
    for (px = 0, x = rx; px < cx; px++, x += sx+1)
    {
      if (px == 0 && py == 0) Cursor.printChar(x,y,ASCII::LINE_UL);
      else if (py == 0) Cursor.printChar(x,y,ASCII::LINE_HD);
      else if (px == 0) Cursor.printChar(x,y,ASCII::LINE_VR);
      else Cursor.printChar(x,y,ASCII::LINE_HV);
      for (i = x+1; i <= x+sx; i++) Cursor.printChar(i,y,ASCII::LINE_H);
      for (i = y+1; i <= y+sy; i++) Cursor.printChar(x,i,ASCII::LINE_V);
    }
    if (py == 0) Cursor.printChar(x,y,ASCII::LINE_UR);
    else Cursor.printChar(x,y,ASCII::LINE_VL);
    for (int i = y+1; i <= y+sy; i++) Cursor.printChar(x,i,ASCII::LINE_V);
  }
  for (int px = 0, x = rx; px < cx; px++, x += sx+1)
  {
    if (px == 0) Cursor.printChar(x,y,ASCII::LINE_DL);
    else Cursor.printChar(x,y,ASCII::LINE_HU);
    for (int i = x+1; i <= x+sx; i++) Cursor.printChar(i,y,ASCII::LINE_H);
  }
  Cursor.printChar(x,y,ASCII::LINE_DR);
}

// Print custom grid using box drawing characters with only lines
// Set position, number of columns and rows, size of column and row
void printGridOutside(int x,int y,int cx,int cy,int sx,int sy)
{
  int px, py, rx = x;
  for (py = 0; py < cy; py++, y += sy+1)
  {
    for (px = 0, x = rx; px < cx; px++, x += sx+1)
    {
      if (px == 0 && py == 0) Cursor.printChar(x,y,ASCII::LINE_UL);
      else if (py == 0) Cursor.printChar(x,y,ASCII::LINE_HD);
      else if (px == 0) Cursor.printChar(x,y,ASCII::LINE_VR);
      else Cursor.printChar(x,y,ASCII::LINE_HV);
    }
    if (py == 0) Cursor.printChar(x,y,ASCII::LINE_UR);
    else Cursor.printChar(x,y,ASCII::LINE_VL);
  }
  for (px = 0, x = rx; px < cx; px++, x += sx+1)
  {
    if (px == 0) Cursor.printChar(x,y,ASCII::LINE_DL);
    else Cursor.printChar(x,y,ASCII::LINE_HU);
  }
  Cursor.printChar(x,y,ASCII::LINE_DR);
}

// Print custom grid using box drawing characters with corners only
// Set position, number of columns and rows, size of column and row
void printGridInside(int x,int y,int cx,int cy,int sx,int sy)
{
  int px,py,i,rx = x;
  for (py = 0; py < cy; py++, y += sy+1)
  {
    for (px = 0, x = rx; px < cx; px++, x += sx+1)
    {
      for (i = x+1; i <= x+sx; i++) Cursor.printChar(i,y,ASCII::LINE_H);
      for (i = y+1; i <= y+sy; i++) Cursor.printChar(x,i,ASCII::LINE_V);
    }
    for (i = y+1; i <= y+sy; i++) Cursor.printChar(x,i,ASCII::LINE_V);
  }
  for (px = 0, x = rx; px < cx; px++, x += sx+1)
    for (i = x+1; i <= x+sx; i++)
      Cursor.printChar(i,y,ASCII::LINE_H);
}

int main()
{
  Window.setSizePixels(940,560); // set window size
  Cursor.setFontSize(30); // set console font size
  Window.hideBlinking(); // hide blinking when printing
  Window.hideScrollbars(); // hide right and bottom scrollbars
  Window.hideSelection(); // hide selection when pressing left click

  Cursor.setColor(YELLOW); // set printing color to yellow
  printFrameDouble(0,0,35,17); // print yellow border

  Cursor.setColor(WHITE); // set printing color to white
  printGrid(2,1,8,8,3,1); // print 8x8 grid with size 3x1

  Cursor.setColor(LIGHTGREEN); // set printing color to green and print Hobit
  printText(40,22,"\"Good Morning!\" said Bilbo, and he meant it.\n \
  The sun was shining, and the grass was very green.\n \
  But Gandalf looked at him from under long bushy eyebrows\n \
  that stuck out further than the brim of his shady hat.", MIDDLE, CENTER);
  Cursor.setColor(GRAY); // set printing color to green
  printLine(40,22,"X"); // print anchor point of the text

  Cursor.setColor(WHITE); // set printing color to white
  while(!Keyboard.get(VK_ESCAPE)) // while escape key isn't pressed
  {
    Cursor.setPosition(5,20); // print mouse position in pixels
    std::cout << Mouse.getX() << "  " << Mouse.getY() << "   ";
    Cursor.setPosition(5,21); // print mouse position in characters
    std::cout << Mouse.getColumns() << "   " << Mouse.getRows() << "   ";
    Keyboard.wait(30);
  }
  return 0;
}
