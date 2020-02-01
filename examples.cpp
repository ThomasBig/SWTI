#include <iostream>
#include <sstream>
#include <algorithm> // std::count
#include "swti/swti.hpp"

////////////////////////////////////////////////////////////////
//                    MORE SWTI EXAMPLES                      //
//                 chessboard and mouse input                 //
////////////////////////////////////////////////////////////////

// Aligns for printing text
enum HAlign {LEFT, CENTER, RIGHT};
enum VAlign {TOP, MIDDLE, DOWN};

// Write text on position with horizontal and vertical align
void printText(int x, int y, std::string text, HAlign halign=LEFT, VAlign valign=TOP)
{
  // position variables
  int xx, yy;
  std::string line;

  // count end of lines
  int cnt = std::count(text.begin(), text.end(), '\n');

  // move text based on vertical align
  switch (valign)
  {
    case TOP:
      yy = 0; break;
    case MIDDLE:
      yy = cnt / 2; break;
    case DOWN:
      yy = cnt; break;
  }

  // print all lines seperately
  std::istringstream separate(text);
  while(std::getline(separate, line))
  {
    // count number of tabs
    int tabs = 7 * std::count(line.begin(), line.end(), '\t');

    // move text based on horizontal align
    switch (halign)
    {
      case LEFT:
        xx = 0; break;
      case CENTER:
        xx = (line.length() + tabs - 1) / 2; break;
      case RIGHT:
        xx = line.length() + tabs - 1; break;
    }

    // move text based on horizontal align
    Cursor.setPosition(x - xx, (y++) - yy);
    std::cout << line;
  }
}

// Print rectangle using double dashed box drawing characters
// Set position and width and height in columns and rows
void printFrame(int x, int y, int columns, int rows)
{
  // corners
  Cursor.printChar(x, y, DLINE_UR);
  Cursor.printChar(x, y + rows + 1, DLINE_DR);
  Cursor.printChar(x + columns + 1, y, DLINE_UL);
  Cursor.printChar(x + columns + 1, y + rows + 1, DLINE_DL);

  // horizontal bars
  for (int i = x + 1; i <= x + columns; i++)
  {
    Cursor.printChar(i, y, DLINE_H);
    Cursor.printChar(i, y + rows + 1, DLINE_H);
  }

  // vertical bars
  for (int i = y + 1; i <= y + rows; i++)
  {
    Cursor.printChar(x, i, DLINE_V);
    Cursor.printChar(x + columns + 1, i, DLINE_V);
  }
}

// Print custom grid using box drawing characters
// Set position, number of columns and rows, size of column and row
void printGrid(int x, int y, int cx, int cy, int sx, int sy)
{
  int px, py, i, rx = x;
  for (py = 0; py < cy; py++, y += sy + 1)
  {
    for (px = 0, x = rx; px < cx; px++, x += sx + 1)
    {
      if (px == 0 && py == 0)
        Cursor.printChar(x, y, LINE_UR);
      else if (py == 0)
        Cursor.printChar(x, y, LINE_HD);
      else if (px == 0)
        Cursor.printChar(x, y, LINE_VR);
      else
        Cursor.printChar(x, y, LINE_HV);

      for (i = x + 1; i <= x + sx; i++)
        Cursor.printChar(i, y, LINE_H);

      for (i = y + 1; i <= y + sy; i++)
        Cursor.printChar(x, i, LINE_V);
    }
    Cursor.printChar(x, y, (py == 0) ? LINE_UL : LINE_VL);
    for (int i = y + 1; i <= y + sy; i++)
      Cursor.printChar(x, i, LINE_V);
  }
  for (int px = 0, x = rx; px < cx; px++, x += sx + 1)
  {
    Cursor.printChar(x, y, (px == 0) ? LINE_DR : LINE_HU);
    for (int i = x + 1; i <= x + sx; i++)
      Cursor.printChar(i, y, LINE_H);
  }
  Cursor.printChar(x, y, LINE_DL);
}

class Piece
{
private:
  int x, y;    // logical position
  char type;   // print character
  Color color; // in color
  int state;   // piece state - idle or moving
public:
  Piece(int x, int y, char type, Color color) :
    x(x), y(y), type(type), color(color), state(0) { show(); }
  Piece() : Piece(0, 0, 'X', WHITE) {}
  int getColumns()
    { return 4 + x * 4; }
  int getRows()
    { return 2 + y * 2; }
  void show()
    {  Cursor.printChar(getColumns(), getRows(), type, color); }
  void drag()
  {
    if (Keyboard.getPressed(VK_LBUTTON) && abs(Mouse.getColumns() - getColumns()) < 3  \
     && abs(Mouse.getRows() - getRows()) < 2)
      state = 1;
    if (state == 1 && !Keyboard.getReleased(VK_LBUTTON))
    {
      Cursor.printBlank(getColumns(), getRows());
      x = (Mouse.getColumns() - 3) / 4; y = (Mouse.getRows() - 2) / 2;
      x = (std::min)((std::max)(x, 0), 7); y = (std::min)((std::max)(y, 0), 7);
      Cursor.printChar(getColumns(), getRows(), type, LIGHTYELLOW);
    }
    else if (state == 1)
    {
      state = 0;
    }
  }
};

int main()
{
  int scw = Window.getScreenWidth();
  int sch = Window.getScreenHeight();
  Window.setSizePixels(scw / 2, sch / 2); // set window size
  Cursor.setFontSize(24); // set console font size
  Window.hideBlinking(); // hide blinking when printing
  Window.hideScrollbars(); // hide right and bottom scrollbars
  Window.hideSelection(); // hide selection when pressing left click
  Window.hideResize(); // hide resizing window with mouse

  Cursor.setColor(YELLOW); // set printing color to yellow
  printFrame(0, 0, 35, 17); // print yellow border

  Cursor.setColor(WHITE); // set printing color to white
  printGrid(2, 1, 8, 8, 3, 1); // print 8x8 grid with size 3x1
  Piece pieces[4] = { Piece(2, 2, 'X', LIGHTRED),   // populate game board with pieces
                      Piece(2, 5, 'Y', LIGHTRED),
                      Piece(5, 2, 'X', LIGHTBLUE),
                      Piece(5, 5, 'Y', LIGHTBLUE) };

  Cursor.setColor(LIGHTGREEN); // set printing color to green and print Hobit
  printText(40, 22, "\"Good Morning!\" said Bilbo, and he meant it.\n \
  The sun was shining, and the grass was very green.\n \
  But Gandalf looked at him from under long bushy eyebrows\n \
  that stuck out further than the brim of his shady hat.", CENTER, MIDDLE);
  Cursor.printChar(40, 22, 'X', GRAY); // print anchor point of the text

  Cursor.setColor(WHITE); // set printing color to white
  while(!Keyboard.get(VK_ESCAPE)) // while escape key isn't pressed
  {
    // show game pieces and drag them using mouse
    for (auto& piece: pieces)
      piece.show();
    for (auto& piece: pieces)
      piece.drag();

     // print mouse position in pixels
    Cursor.setColor(WHITE);
    Cursor.setPosition(5, 20);
    std::cout << Mouse.getX() << "  " << Mouse.getY() << "   ";

    // print mouse position in characters
    Cursor.setPosition(5, 21);
    std::cout << Mouse.getColumns() << "   " << Mouse.getRows() << "   ";
    Keyboard.wait(30);
  }
  return 0;
}
