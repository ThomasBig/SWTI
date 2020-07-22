#include "swti.hpp"

////////////////////////////////////////////////////////////////
//                  MINGW MISSING FUNCTIONS                   //
//                   console font, set, get                   //
////////////////////////////////////////////////////////////////

// if we do not have font structure
#ifndef CONSOLE_FONT_INFOEX

// define structure for console fonts
typedef struct _CONSOLE_FONT_INFOEX // typedef is neccessary
{
    ULONG cbSize; // size of font
    DWORD nFont;  // id of font
    COORD dwFontSize; // size in X and Y
    UINT  FontFamily; // style of font
    UINT  FontWeight; // normal 400, bold 700
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

// get those functions from C language
extern "C"
{
  BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput,
    BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
  BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput,
    BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
}

#endif // CONSOLE_FONT_INFOEX

// If user has a GNU library then define missing functions
#if defined(__GLIBCXX__) || defined(__GLIBCPP__)

// GNU uses standard C++ function to copy wstrings
const auto& widecpy = wcscpy;  // define function alias
auto strlpc(std::string str) // gnu better works with raw string
{
	return str;
}

#else // when using a visual studio compiler

// use visual function to copy wide strings
// C++ can't define alias functions with template variables
auto widecpy(wchar_t* dest, const wchar_t* src)
{
	return wcscpy_s(dest, 32, src); // 32 is the maximum size of wchar_t
}

// convert string to wide string for later use
// visual studio works better with wide strings
auto strlpc(std::string str)
{
	return std::wstring(str.begin(), str.end());
}

#endif // end of compiler dependant functions


////////////////////////////////////////////////////////////////
//                     CREATE ALL OBJECTS                     //
//              Cursor, Keyboard, Window, Mouse               //
////////////////////////////////////////////////////////////////

// create these variables for module purporse
SWTI_Window swti_window = SWTI_Window::getInstance();
SWTI_Cursor swti_cursor = SWTI_Cursor::getInstance();
SWTI_Keyboard swti_keyboard = SWTI_Keyboard::getInstance();
SWTI_Mouse swti_mouse = SWTI_Mouse::getInstance();

// create these easy to use names for user
SWTI_Window& Window = swti_window;
SWTI_Cursor& Cursor = swti_cursor;
SWTI_Keyboard& Keyboard = swti_keyboard;
SWTI_Mouse& Mouse = swti_mouse;

////////////////////////////////////////////////////////////////
//              MODULE STRUCTURES AND VARIABLES               //
//                 constants, error handling                  //
////////////////////////////////////////////////////////////////

// default constants
const int SWTI_ERROR = -1; // error value for int based functions
const int SWTI_DELAY = 100; // delay in ms used to slower some functions

// macro for printing errors, if error occurs print SWTI and WINAPI function
#define SWTI_PERR(bSuccess, strFunc, strApi) { if (!bSuccess) \
  std::cout << __FILE__ << " SWTI Error " << GetLastError() << " from " << \
  strFunc << " called by " << strApi << " on line "<< __LINE__ << std::endl; }

// macro for printing int based errors, print SWTI and WINAPI function
#define SWTI_PERRI(iSuccess, strFunc, strApi) { if (iSuccess == SWTI_ERROR) \
  std::cout << __FILE__ << " SWTI Error " << GetLastError() << " from " << \
  strFunc << " called by " << strApi << " on line "<< __LINE__ << std::endl; }

////////////////////////////////////////////////////////////////
//                      CURSOR FUNCTIONS                      //
//              setPosition, setColor, printChar              //
////////////////////////////////////////////////////////////////

// get position of cursor in X, use screen buffer
// for repeated usage make a local variable instead
// if the function fails, it returns SWTI_ERROR
int SWTI_Cursor::getX()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &csbi);
  SWTI_PERR(result, "Cursor.getX", "GetConsoleScreenBufferInfo");
  return result ? csbi.dwCursorPosition.X : SWTI_ERROR;
}

// get position of cursor in Y, use screen buffer
// for repeated usage make a local variable instead
// if the function fails, it returns SWTI_ERROR
int SWTI_Cursor::getY()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &csbi);
  SWTI_PERR(result, "Cursor.getY", "GetConsoleScreenBufferInfo");
  return result ? csbi.dwCursorPosition.Y : SWTI_ERROR;
}

// get foreground printing color of cursor
// uses private variable rather than screen buffer
// this function doesn't return errors
Color SWTI_Cursor::getColorForeground()
{
  CONSOLE_SCREEN_BUFFER_INFO info;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &info);
  SWTI_PERR(result, "Cursor.getColorForeground", "GetConsoleScreenBufferInfo");
  return result ? Color(info.wAttributes % 16) : CURRENT;
}

// get background printing color of cursor
// uses private variable rather than screen buffer
// this function doesn't return errors
Color SWTI_Cursor::getColorBackground()
{
  CONSOLE_SCREEN_BUFFER_INFO info;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &info);
  SWTI_PERR(result, "Cursor.getColorForeground", "GetConsoleScreenBufferInfo");
  return result ? Color(info.wAttributes / 16) : CURRENT;
}

// set cursor position, check correct position
// set the position using handle and function in windows.h
// if the function fails, it returns false
bool SWTI_Cursor::setPosition(int x, int y)
{
  if (x < 0 || y < 0 || x >= swti_window.getColumns() || y >= swti_window.getRows())
    return true; // nothing to do
  COORD point; point.X = x; point.Y = y;
  BOOL result = SetConsoleCursorPosition(hOutput, point);
  SWTI_PERR(result, "Cursor.setPosition", "SetConsoleCursorPosition");
  return result;
}

// set cursor color, print in different colors than white
// color is an enum defined in header file, e.x. SetCursorColor(RED);
// tip: you can also specify the background color
bool SWTI_Cursor::setColor(Color foreground, Color background)
{
  // CURRENT is a special color, it is the color in use
  if (foreground == CURRENT)
  {
    foreground = getColorForeground();
    BOOL result = foreground == CURRENT ? FALSE : TRUE;
    SWTI_PERR(result, "Cursor.setColor", "Cursor.getColorForeground");
  }

  // CURRENT is a special color, it is the color in use
  if (background == CURRENT)
  {
    background = getColorBackground();
    BOOL result = background == CURRENT ? FALSE : TRUE;
    SWTI_PERR(result, "Cursor.setColor", "Cursor.getColorBackground");
  }

  WORD wcol = foreground + 16 * background;
	BOOL result = SetConsoleTextAttribute(hOutput, wcol);
  SWTI_PERR(result, "Cursor.setColor", "SetConsoleTextAttribute");
  return result;
}

// print one char at specified position
// character can be any ascii character
// use cout for printing more characterss
bool SWTI_Cursor::printChar(int x, int y, int character, Color color)
{
  bool result = SWTI_Cursor::setPosition(x, y);
  SWTI_PERR(result, "Cursor.printChar", "Cursor.setPosition");
  if (color != CURRENT)
      result = SWTI_Cursor::setColor(color);
  SWTI_PERR(result, "Cursor.printChar", "Cursor.setColor");
  if (result) std::cout << (char) character;
  return result;
}

// print one blank at specified position
// fast function to hide a character
// use cout for hiding more characterss
bool SWTI_Cursor::printBlank(int x, int y)
{
  bool result = SWTI_Cursor::setPosition(x, y);
  SWTI_PERR(result, "Cursor.printBlank", "Cursor.setPosition");
  if (result) std::cout << ' ';
  return result;
}

// clear full window, slow function not to be used in a loop
// to hide only few characters, use printBlank()
bool SWTI_Cursor::clearScreen()
{
  COORD coordScreen = {0, 0};
  DWORD cCharsWritten;
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  DWORD dwConSize;
  BOOL rgcsbi, rfcoc, retgcsbi, rfcoa;
  bool setpos;

  // get number of chars
  rgcsbi = GetConsoleScreenBufferInfo(hOutput, &csbi);
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

  // clear entire screen with blanks, set text atribute to current atribute
  rfcoc = FillConsoleOutputCharacter(hOutput,
    (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
  retgcsbi = GetConsoleScreenBufferInfo(hOutput, &csbi); // set text atribute
  rfcoa = FillConsoleOutputAttribute(hOutput,
    csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
  setpos = SWTI_Cursor::setPosition(0, 0); // reset position

  // print any error
  SWTI_PERR(rgcsbi, "Cursor.clearScreen", "GetConsoleScreenBufferInfo");
  SWTI_PERR(rfcoc, "Cursor.clearScreen", "FillConsoleOutputCharacter");
  SWTI_PERR(retgcsbi, "Cursor.clearScreen", "GetConsoleScreenBufferInfo");
  SWTI_PERR(rfcoa, "Cursor.clearScreen", "FillConsoleOutputAttribute");
  SWTI_PERR(setpos, "Cursor.clearScreen", "Cursor.setPosition");

  return rgcsbi && rfcoc && retgcsbi && rfcoa && setpos;
}

// get cursor width in pixels, if it fails, it returns 0
// use font structucture and winapi font functions
int SWTI_Cursor::getFontWidth()
{
  CONSOLE_FONT_INFOEX font={};
  font.cbSize = sizeof(font);
  BOOL result = GetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.getFontWidth", "GetCurrentConsoleFontEx");
  return result ? font.dwFontSize.X : SWTI_ERROR;
}

// get cursor height in pixels, if it fails, it returns 0
// use font structucture and winapi font functions
int SWTI_Cursor::getFontHeight()
{
  CONSOLE_FONT_INFOEX font={};
  font.cbSize = sizeof(font);
  BOOL result = GetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.getFontHeight", "GetCurrentConsoleFontEx");
  return result ? font.dwFontSize.Y : SWTI_ERROR;
}

// get cursor height in custom units
// can be differ from previous size due to change in console size
int SWTI_Cursor::getFontSize()
{
  int size = SWTI_Cursor::getFontHeight();
  int wh = swti_window.getHeight();
  SWTI_PERRI(size, "Cursor.getFontHeight", "Cursor.getFontSize");
  SWTI_PERRI(wh, "Cursor.setFontSize", "Window.getHeight");
  return (size && wh) ? (size * 750) / wh : SWTI_ERROR;
}

// get font name as string
// some fonts can contain undefined letters
std::string SWTI_Cursor::getFontType()
{
  CONSOLE_FONT_INFOEX font={};
  font.cbSize = sizeof(font);
  BOOL result = GetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.getFontType", "GetCurrentConsoleFontEx");
  std::wstring wchar(font.FaceName);
  std::string fontName(wchar.begin(), wchar.end());
  return result ? fontName : "";
}

// set font type using font name
// changes window size proportionally
// font name is converted to wstring
bool SWTI_Cursor::setFontType(const std::string name)
{
  std::wstring wchar = std::wstring(name.begin(), name.end());
  const wchar_t* wcs = wchar.c_str();
  int fontWidth = SWTI_Cursor::getFontWidth();
  int fontHeight = SWTI_Cursor::getFontHeight();
  SWTI_PERRI(fontWidth, "Cursor.setFontType", "Cursor.getFontWidth");
  SWTI_PERRI(fontHeight, "Cursor.setFontType", "Cursor.getFontHeight");

  CONSOLE_FONT_INFOEX font={};
  font.cbSize = sizeof(font);
  font.dwFontSize.X = fontWidth;
  font.dwFontSize.Y = fontHeight;
  widecpy(font.FaceName, wcs);  // use renamed wcscpy function
  BOOL result = SetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.setFontType", "SetCurrentConsoleFontEx");
  return result;
}

// set new height of console font
// set size proportionally to screen height
// width will be calculated automatically
bool SWTI_Cursor::setFontSize(int size)
{
  int wh = swti_window.getHeight();
  SWTI_PERRI(wh, "Cursor.setFontSize", "Window.getHeight");
  size = (wh * size) / 750;
  bool result = SWTI_Cursor::setFontPixels(0, size);
  SWTI_PERR(result, "Cursor.setFontSize", "Cursor.setFontPixels");
  return result;
}

// set cursor font size, good for export to different monitor sizes
// tip: if one argument is 0, it is automatically calculated
// use the font structure and extern C function
bool SWTI_Cursor::setFontPixels(int width, int height)
{
  int wwidth = swti_window.getWidth(); // save window width
  int wheight = swti_window.getHeight(); // save window height
  const std::string fontName = SWTI_Cursor::getFontType(); // save font name
  BOOL bfontsize = fontName.size() > 0 ? TRUE : FALSE;
  SWTI_PERR(bfontsize, "Cursor.setFontPixels", "Cursor.getFontType");
  SWTI_PERRI(wwidth, "Cursor.setFontPixels", "Window.getWidth");
  SWTI_PERRI(wheight, "Cursor.setFontPixels", "Window.getHeight");

  CONSOLE_FONT_INFOEX font={};
  font.cbSize = sizeof(font);
  font.dwFontSize.X = width;
  font.dwFontSize.Y = height;
  BOOL result = SetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.setFontPixels", "SetCurrentConsoleFontEx");
  bool res = SWTI_Cursor::setFontType(fontName);
  SWTI_PERR(res, "Cursor.setFontPixels", "SetCurrentConsoleFontEx");

  Sleep(SWTI_DELAY);  // slow winapi function needs a delay
  bool ret = swti_window.setSizePixels(wwidth,wheight);  // reset size
  SWTI_PERR(ret,"Cursor.setFontPixels", "Window.setSizePixels");
  ret &= (bool) result; // ret is true if setFontType and setSizePixels are true
  return ret;
}

// set font size based on specified characters in window
// good for export to different monitor sizes
// use the font structure and extern C function
bool SWTI_Cursor::setFontChars(int columns, int rows)
{
  int width = swti_window.getWidth();
  int height = swti_window.getHeight();
  bool rsfp = SWTI_Cursor::setFontPixels(width/columns, height/rows);
  bool rssp = swti_window.setSizePixels(width, height);

  SWTI_PERR(width, "Cursor.setFontChars", "Window.getWidth");
  SWTI_PERR(height, "Cursor.setFontChars", "Window.getHeight");
  SWTI_PERR(rsfp, "Cursor.setFontChars", "Cursor.setFontPixels");
  SWTI_PERR(rssp, "Cursor.setFontChars", "Window.setSizePixels");

  return rsfp && rssp;
}

// get instance of cursor
// only one object is needed
SWTI_Cursor& SWTI_Cursor::getInstance()
{
  static SWTI_Cursor instance; // guaranteed to be destroyed
  return instance; // instantiated on first use
}

// private constructor of cursor
// clear screen for the first time
// get handles for cursor
SWTI_Cursor::SWTI_Cursor()
{
  hOutput = GetStdHandle(STD_OUTPUT_HANDLE);  // used in position functions

  if (hOutput == INVALID_HANDLE_VALUE)
    {SWTI_PERR(FALSE, "Cursor Output", "GetStdHandle");}

  SWTI_Cursor::clearScreen();
}

// private destructor of cursor
// handles are closed by os
SWTI_Cursor::~SWTI_Cursor() { }


////////////////////////////////////////////////////////////////
//                     KEYBOARD FUNCTIONS                     //
//                get, getPressed, getReleased                //
////////////////////////////////////////////////////////////////

// check if key is currently down, try get(VK_LEFT) or get('A')
// using the number 0x8000 to get the most important bit
// this function does not return error value
bool SWTI_Keyboard::get(int key)
{
  return GetKeyState(key) & 0x8000;
}

// test if key was pressed since the last call of Keyboard.wait()
// use custom keyboard storage for fast check
// this function does not return error value
bool SWTI_Keyboard::getPressed(int key)
{
  return !pKeys[key] && nKeys[key];
}

// test if key was released since the last call of Keyboard.wait()
// use custom keyboard storage for fast check
// this function does not return error value
bool SWTI_Keyboard::getReleased(int key)
{
  return pKeys[key] && !nKeys[key];
}

// pause application for ticks per second
// higher values means faster program
// good for slowing fast input checks
bool SWTI_Keyboard::wait(unsigned int ticks)
{
  if (ticks > 0)
    Sleep(1000 / ticks);

  BYTE keys[256]; // structure that holds key states
  SWTI_Keyboard::get(VK_RETURN); // one check for GetKeyboardState to work
  BOOL result = GetKeyboardState(keys);
  SWTI_PERR(result, "Keyboard.wait", "GetKeyboardState");
  if (!result) return false;

  // assign previous and new keys
  for (int key = 0; key < 256; key++)
  {
    pKeys[key] = nKeys[key];
    nKeys[key] = keys[key] >> 7;
  }

  return true;
}

// pause until user presses a key
// returns false if error occured
bool SWTI_Keyboard::waitUser()
{
  bool result;
  while(true)
  {
    result = SWTI_Keyboard::wait(SWTI_DELAY);
    SWTI_PERR(result, "Keyboard.waitUser", "Keyboard.wait");

    // go through all the keys, end function if a new key was pressed
    for (int i = 0; i < 256; i++)
    {
      if (nKeys[i] == 1 && nKeys[i] != pKeys[i])
      {
        return true;
      }
    }
  }
}

// get instance of keyboard
// only one object is needed
SWTI_Keyboard& SWTI_Keyboard::getInstance()
{
  static SWTI_Keyboard instance; // guaranteed to be destroyed
  return instance; // instantiated on first use
}

// private constructor of keyboard
// set all 256 keys to zero
SWTI_Keyboard::SWTI_Keyboard() : nKeys(), pKeys() { }

// private destructor of keyboard
// all variables are destroyed automatically
SWTI_Keyboard::~SWTI_Keyboard() { }


////////////////////////////////////////////////////////////////
//                      MOUSE FUNCTIONS                       //
//                    getColumns, getRows                     //
////////////////////////////////////////////////////////////////

// x position of mouse in console window in pixels
// can return negatives and numbers larger than screen width
// prints error message when error occurs
int SWTI_Mouse::getX()
{
  POINT pt;
  BOOL result = GetCursorPos(&pt);
  int bh = swti_window.getBarHeight();
  int x = swti_window.getX();

  SWTI_PERR(result, "Mouse.getX", "GetCursorPos");
  SWTI_PERRI(bh, "Mouse.getX", "Window.getBarHeight");
  SWTI_PERRI(x, "Mouse.getX", "Window.getX");
  if (x == SWTI_ERROR || result == SWTI_ERROR)
    return SWTI_ERROR;

  x = pt.x - x;
  return bh ? x - 8 : x;
}

// y position of mouse in console window in pixels
// can return negatives and numbers larger than screen height
// prints error message when error occurs
int SWTI_Mouse::getY()
{
  POINT pt;
  BOOL result = GetCursorPos(&pt);
  int bh = swti_window.getBarHeight();
  int y = swti_window.getY();

  SWTI_PERR(result, "Mouse.getY", "GetCursorPos");
  SWTI_PERRI(bh, "Mouse.getY", "Window.getBarHeight");
  SWTI_PERRI(y, "Mouse.getY", "Window.getY");

  if (y == SWTI_ERROR || bh == SWTI_ERROR || result == SWTI_ERROR)
    return SWTI_ERROR;

  y = pt.y - y;
  return bh ? y + 8 : y;
}

// x position of mouse in console window in columns
// when console font is unknown use 13 as default
// use for hovering or clicking on text in console
int SWTI_Mouse::getColumns()
{
  int x = SWTI_Mouse::getX();
  int cw = swti_cursor.getFontWidth();
  SWTI_PERRI(cw, "Mouse.getColumns", "Cursor.getFontWidth");
  x = (cw != SWTI_ERROR) ? x / cw : x / 13; // convert to columns
  return x;
}

// y position of mouse in console window in rows
// when console font is unknown use 24 as default
// use for hovering or clicking on text in console
int SWTI_Mouse::getRows()
{
  int y = SWTI_Mouse::getY();
  int ch = swti_cursor.getFontHeight();
  SWTI_PERRI(ch, "Mouse.getRows", "Cursor.getFontHeight");
  y = (ch != SWTI_ERROR) ? y / ch : y / 24; // convert to columns
  return y;
}

// get instance of mouse
// only one object is needed
SWTI_Mouse& SWTI_Mouse::getInstance()
{
  static SWTI_Mouse instance; // guaranteed to be destroyed
  return instance; // instantiated on first use
}

// private constructor of mouse
SWTI_Mouse::SWTI_Mouse() { }

// private destructor of mouse
// all variables are destroyed automatically
SWTI_Mouse::~SWTI_Mouse() { }


////////////////////////////////////////////////////////////////
//                      WINDOW FUNCTIONS                      //
//    setFullscreenBorderless, setPosition, setSizePixels     //
////////////////////////////////////////////////////////////////

// Position of left part of console window in pixels
int SWTI_Window::getX()
{
  RECT rect = {};
  BOOL result = GetWindowRect(hWindow, &rect);
  SWTI_PERR(result, "Window.getX", "GetClientRect");
  return result ? rect.left : SWTI_ERROR;
}

// Position of top part of console window in pixels
int SWTI_Window::getY()
{
  RECT rect = {};
  BOOL result = GetWindowRect(hWindow, &rect);
  SWTI_PERR(result, "Window.getY", "GetClientRect");
  int bh = getBarHeight();
  SWTI_PERRI(bh, "Window.getY", "Window.getBarHeight");
  return result ? (rect.top + bh) : SWTI_ERROR;
}

// Width of console window in pixels
// get client window rectangle
int SWTI_Window::getWidth()
{
  RECT rect = {};
  BOOL result = GetClientRect(hWindow, &rect);
  SWTI_PERR(result, "Window.getWidth", "GetClientRect");
  return result ? (rect.right) : SWTI_ERROR;
}

// Height of console window in pixels
// get client window rectangle
int SWTI_Window::getHeight()
{
  RECT rect = {};
  BOOL result = GetClientRect(hWindow, &rect);
  SWTI_PERR(result, "Window.getHeight", "GetClientRect");
  return result ? (rect.bottom) : SWTI_ERROR;
}

// width of the console window in pixels
int SWTI_Window::getColumns()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &csbi);
  SWTI_PERR(result, "Window.getColumns", "GetConsoleScreenBufferInfo");
  return result ? (csbi.srWindow.Right - csbi.srWindow.Left + 1) : SWTI_ERROR;
}

// height of the console window in pixels
int SWTI_Window::getRows()
{
  CONSOLE_SCREEN_BUFFER_INFO csbi;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &csbi);
  SWTI_PERR(result, "Window.getRows", "GetConsoleScreenBufferInfo");
  return result ? (csbi.srWindow.Bottom - csbi.srWindow.Top + 1) : SWTI_ERROR;
}

// Size of console title bar
// Returns positive number if window is in borderless fullscreen
int SWTI_Window::getBarHeight()
{
  BOOL rwr, rcr;
  RECT screen, client;
  rwr = GetWindowRect(hWindow, &screen);
  rcr = GetClientRect(hWindow, &client);
  SWTI_PERR(rwr, "Window.getBarHeight", "GetWindowRect");
  SWTI_PERR(rcr, "Window.getBarHeight", "GetClientRect");
  int size = (screen.bottom - screen.top) - (client.bottom - client.top);
  if (size != 0) size -= 8;
  return (rwr && rcr) ? size : SWTI_ERROR;
}

// get width of monitor in pixels
int SWTI_Window::getScreenWidth()
{
  int width = GetSystemMetrics(SM_CXSCREEN);
  SWTI_PERRI(width, "Cursor.getScreenWidth", "GetSystemMetrics");
  return width > 0 ? width : SWTI_ERROR;
}

// get height of monitor in pixels
int SWTI_Window::getScreenHeight()
{
  int height = GetSystemMetrics(SM_CYSCREEN);
  SWTI_PERRI(height, "Cursor.getScreenHeight", "GetSystemMetrics");
  return height > 0 ? height : SWTI_ERROR;
}

// Set window position in pixels
bool SWTI_Window::setPositionPixels(int x, int y)
{
  UINT flags = SWP_NOSIZE | SWP_NOZORDER;
  BOOL result = SetWindowPos(hWindow, 0, x, y, 0, 0, flags);
  SWTI_PERR(result, "Window.setPositionPixels", "SetWindowPos");
  bool ret = result;
  return ret;
}

// Set position to center of window
// Set position to center of window
bool SWTI_Window::setPositionCenter()
{
  int swidth = SWTI_Window::getScreenWidth();
  int sheight = SWTI_Window::getScreenHeight();
  int wwidth = SWTI_Window::getWidth();
  int wheight = SWTI_Window::getHeight();

  SWTI_PERRI(swidth,  "Window.setPositionCenter", "Window.getScreenWidth");
  SWTI_PERRI(sheight, "Window.setPositionCenter", "Window.getScreenHeight");
  SWTI_PERRI(wwidth,  "Window.setPositionCenter", "Window.getWidth");
  SWTI_PERRI(wheight, "Window.setPositionCenter", "Window.getHeight");

  int posx = swidth / 2 - wwidth / 2;
  int posy = sheight / 2 - wheight / 2;
  bool result = SWTI_Window::setPositionPixels(posx, posy);
  SWTI_PERR(result, "Window.setPositionCenter", "Window.setPositionPixels");
  return result;
}

// Set window size in pixels
bool SWTI_Window::setSizePixels(int width, int height)
{
  UINT flags = SWP_NOZORDER | SWP_NOMOVE;
  BOOL result = SetWindowPos(hWindow, 0, 0, 0, width, height, flags);
  bool center = SWTI_Window::setPositionCenter();
  SWTI_PERR(result, "Window.setSizePixels", "SetWindowPos");
  SWTI_PERR(center, "Window.setSizePixels", "Window.setPositionCenter");
  bool ret = result;
  return ret && center;
}

// set the size of console window in Columns and Rows
// if you want fullscreen, use setFullscreenWindow() instead
// return true if resize was successful
bool SWTI_Window::setSizeChars(int columns, int rows)
{
  // create coordinates
  COORD coord;
  coord.X = columns;
  coord.Y = rows;

  // build the rectangle
  SMALL_RECT wsize;
  wsize.Left = 0;
  wsize.Top = 0;
  wsize.Right = columns - 1;
  wsize.Bottom = rows - 1;

  // first set console window to minimal
  SMALL_RECT minimal = {0, 0, 1, 1};

  BOOL rscwim, rscsbi, rscwi;
  rscwim = SetConsoleWindowInfo(hOutput, TRUE, &minimal);
  rscsbi = SetConsoleScreenBufferSize(hOutput, coord);
  rscwi = SetConsoleWindowInfo(hOutput, TRUE, &wsize);

  SWTI_PERR(rscwim, "Window.setSizeChars", "SetConsoleWindowInfo");
  SWTI_PERR(rscsbi, "Window.setSizeChars", "SetConsoleScreenBufferSize");
  SWTI_PERR(rscwi,  "Window.setSizeChars", "SetConsoleWindowInfo");

  Sleep(SWTI_DELAY);

  // move window to center
  bool center = SWTI_Window::setPositionCenter();
  SWTI_PERR(center, "Window.setSizePixels", "Window.setPositionCenter");
  return rscsbi && rscwi;
}

// set the size of console window to maximum size
// warning: this function resets the cursor visibility
bool SWTI_Window::setFullscreenWindow()
{
  int bh = SWTI_Window::getBarHeight();
  SWTI_PERRI(bh, "Window.setFullscreenWindow", "Window.getBarHeight");

  // if window is in borderless mode
  if (bh == 0)
  {
    bool res = SWTI_Window::setFullscreenBorderless(); // press f11
    SWTI_PERR(res, "Window.setFullscreenWindow", "Window.setFullscreenBorderless");
  }

  // set size to maximum window
  BOOL result = ShowWindow(hWindow, SW_MAXIMIZE);
  SWTI_PERR(result, "Window.setFullscreenWindow", "Window.ShowWindow");
  return result;
}

// set the size of console window to maximum size without borders
// warning: this function resets the cursor visibility
bool SWTI_Window::setFullscreenBorderless()
{
  INPUT ip;
  BOOL retpress, retrelease;
  bool result;

  ip.type = INPUT_KEYBOARD;
  ip.ki = {VK_F11, 0, 0, 0, 0}; // prepare f11 in a keyboard structure
  retpress = SendInput(1, &ip, sizeof(INPUT)); // press the F11 key
  SWTI_PERR(retpress, "Window.setFullscreenWindow", "SendInput");

  ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
  retrelease = SendInput(1, &ip, sizeof(INPUT)); // release the F11 key
  SWTI_PERR(retrelease, "Window.setFullscreenWindow", "SendInput");

  Sleep(SWTI_DELAY); // wait for apply
  result = SWTI_Window::hideScrollbars(); // hide scrollbars
  SWTI_PERR(result, "Window.setFullscreenWindow", "Window.hideScrollbars");
  return retpress && retrelease;
}

// set colors for the whole window
// changes all current colors and cursor colors
bool SWTI_Window::setColor(Color foreground, Color background)
{
  if (foreground == CURRENT)
    foreground = swti_cursor.getColorForeground();
  if (background == CURRENT)
    background = swti_cursor.getColorBackground();

  WORD wcol = foreground + 16 * background; // calculate colors
  COORD coordScreen = {0, 0}; // position of first char
  CONSOLE_SCREEN_BUFFER_INFO csbi; // information about console
  DWORD cCharsWritten;  // get number of changed chars
  DWORD dwConSize;  // set number of all chars in screen
  BOOL rgcsbi, rfcoa;  // error handling
  bool result;

  // get number of chars
  rgcsbi = GetConsoleScreenBufferInfo(hOutput, &csbi);
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
  rfcoa = FillConsoleOutputAttribute(hOutput,
    wcol, dwConSize, coordScreen, &cCharsWritten);
  result = swti_cursor.setColor(foreground, background); // set cursor color

  // print any error
  SWTI_PERR(rgcsbi, "Window.setColor", "GetConsoleScreenBufferInfo");
  SWTI_PERR(rfcoa,  "Window.setColor", "FillConsoleOutputAttribute");
  SWTI_PERR(result, "Window.setColor", "Cursor.setColor");

  // return result
  result = result && rgcsbi && rfcoa;
  return result;
}


// set the visibility of cursor as visible
// show the blinking cursor when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::showBlinking()
{
  // set blinking cursor
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 25; // cursor size isn't the font size
  info.bVisible = TRUE; // bVisible is an int
  BOOL result = SetConsoleCursorInfo(hOutput, &info);
  SWTI_PERR(result, "Window.showBlinking", "SetConsoleCursorInfo");
  return result; // return true if everything is alright
}

// set the visibility of cursor as hidden
// hide the blinking cursor when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::hideBlinking()
{
  // set blinking cursor
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 25; // cursor size isn't the font size
  info.bVisible = FALSE; // bVisible is an int
  BOOL result = SetConsoleCursorInfo(hOutput, &info);
  SWTI_PERR(result, "Window.hideBlinking", "SetConsoleCursorInfo");
  return result; // return true if everything is alright
}

// set selection visibility
// show selection when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::showSelection()
{
  DWORD mode; // holds all the console flags
  BOOL rgcm; // store messages from winapi
  BOOL rscm; // store messages from winapi

  // get console information and enable edit mode
  rgcm = GetConsoleMode(hInput, &mode);
  rscm = SetConsoleMode(hInput, mode & ENABLE_QUICK_EDIT_MODE);

  // print any error
  SWTI_PERR(rgcm, "Window.showSelection", "GetConsoleMode");
  SWTI_PERR(rscm, "Window.showSelection", "SetConsoleMode");

  return rgcm & rscm;
}

// set selection visibility
// hide selection when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::hideSelection()
{
  DWORD mode; // holds all the console flags
  BOOL rgcm; // store messages from winapi
  BOOL rscm; // store messages from winapi
  bool result; // return value

  // get console information, disable edit mode
  rgcm = GetConsoleMode(hInput, &mode);
  rscm = SetConsoleMode(hInput, mode & ~ENABLE_QUICK_EDIT_MODE);

  // print any error
  SWTI_PERR(rgcm, "Window.hideSelection", "GetConsoleMode");
  SWTI_PERR(rscm, "Window.hideSelection", "SetConsoleMode");

  result = rgcm & rscm;
  return result;
}

// set resizable mouse controls
// enable user to drag corners and press maximize
bool SWTI_Window::showResize()
{
  // set console information, enable resize
  LONG newinfo = GetWindowLong(hWindow, GWL_STYLE) | WS_MAXIMIZEBOX | WS_SIZEBOX;
  BOOL result = SetWindowLong(hWindow, GWL_STYLE, newinfo);
  SWTI_PERR(result, "Window.hideSelection", "GetConsoleMode");
  return result;
}

// set resizable mouse controls
// prevent user to drag corners and press maximize
bool SWTI_Window::hideResize()
{
  // set console information, disable resize
  LONG newinfo = GetWindowLong(hWindow, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
  BOOL result = SetWindowLong(hWindow, GWL_STYLE, newinfo);
  SWTI_PERR(result, "Window.hideSelection", "GetConsoleMode");
  return result;
}

// show right and bottom scrollbar
// change how many columns and rows can be scrolled
// to hide a scrollbar use hideScrollbars
bool SWTI_Window::showScrollbars(int columns, int rows)
{
  COORD size;
  BOOL result;
  int wgc, wgr;
  bool ret;

  // get current buffer
  wgc = SWTI_Window::getColumns();
  wgr = SWTI_Window::getRows();
  size.X = wgc + columns;
  size.Y = wgr + rows;
  result = SetConsoleScreenBufferSize(hOutput, size);

  // print error messages
  SWTI_PERRI(wgc, "Window.hideSelection", "getColumns");
  SWTI_PERRI(wgr, "Window.hideSelection", "getRows");
  SWTI_PERR(result, "Window.showScrollbars", "SetConsoleScreenBufferSize");

  ret = result && (wgc != SWTI_ERROR) && (wgr != SWTI_ERROR);
  return ret;
}

// hide both scrollbars
bool SWTI_Window::hideScrollbars()
{
  // set scrollbars to zero characters
  bool result = SWTI_Window::showScrollbars(0, 0);
  SWTI_PERR(result, "Cursor.hideScrollbars", "Cursor.showScrollbars");
  return result;
}

// set console name using a string from standart library
bool SWTI_Window::setTitle(std::string title)
{
  // use custom strlpc function that converts and prepares string
  BOOL result = SetConsoleTitle(strlpc(title).c_str());
  SWTI_PERR(result, "Window.setTitle", "SetConsoleTitle");
  return result;
}

// get instance of window
// only one object is needed
SWTI_Window& SWTI_Window::getInstance()
{
  static SWTI_Window instance; // guaranteed to be destroyed
  return instance; // instantiated on first use
}

// private constructor of window
// get handles
SWTI_Window::SWTI_Window()
{
  hWindow = GetConsoleWindow(); // used when moving and resizing
  hOutput = GetStdHandle(STD_OUTPUT_HANDLE);  // for getting rows and columns
  hInput = GetStdHandle(STD_INPUT_HANDLE);  // used for cursor visibility

  if (hWindow == NULL)
    {SWTI_PERR(FALSE, "Window Handle", "GetConsoleWindow");}
  if (hOutput == INVALID_HANDLE_VALUE)
    {SWTI_PERR(FALSE, "Window Output", "GetStdHandle");}
  if (hInput == INVALID_HANDLE_VALUE)
    {SWTI_PERR(FALSE, "Cursor Input", "GetStdHandle");}
}

// destructor of window
// handles are closed by os
SWTI_Window::~SWTI_Window() { }
