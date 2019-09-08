#include "swti.h"

////////////////////////////////////////////////////////////////
//               MINGW MISSING FUNCTIONS                      //
//               console font, set, get                       //
////////////////////////////////////////////////////////////////

/* IF YOU HAVE ERRORS WITH YOUR COMPILER, UNCOMMENT THIS
// default structure for console fonts that isn't defined by MINGW
typedef struct _CONSOLE_FONT_INFOEX // typedef is neccessary
{
    ULONG cbSize; // size of font
    DWORD nFont;  // id of font
    COORD dwFontSize; // size in X and Y
    UINT  FontFamily; // style of font
    UINT  FontWeight; // normal 400, bold 700
    WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

// define functions from WINAPI that aren't in windows.h
extern "C"  // get functions from C language
{
  BOOL WINAPI SetCurrentConsoleFontEx(HANDLE hConsoleOutput,
    BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
  BOOL WINAPI GetCurrentConsoleFontEx(HANDLE hConsoleOutput,
    BOOL bMaximumWindow, PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx);
} END OF MINGW MISSING FUNCTIONS */

////////////////////////////////////////////////////////////////
//            MODULE STRUCTURES AND VARIABLES                 //
//                 handle, macros, font                       //
////////////////////////////////////////////////////////////////

// default constants
const int SWTI_ERROR = -1; // error value for int based functions
const int SWTI_DELAY = 100; // delay in ms used in slow functions to pause a program

// macro for printing errors, if error occurs, print SWTI and WINAPI functions
#define SWTI_PERR(bSuccess, strFunc, strApi) { if (!bSuccess) \
  std::cout << __FILE__ << " SWTI Error " << GetLastError() << " from " << \
  strFunc << " called by " << strApi << " on line "<< __LINE__ << std::endl; }

// macro for printing int based errors, print SWTI and WINAPI functions
#define SWTI_PERRI(iSuccess, strFunc, strApi) { if (iSuccess == SWTI_ERROR) \
  std::cout << __FILE__ << " SWTI Error " << GetLastError() << " from " << \
  strFunc << " called by " << strApi << " on line "<< __LINE__ << std::endl; }

// small function to convert windows BOOL to c++ bool
bool SWTI_BOOL(BOOL b) { return (b ? true : false); }

////////////////////////////////////////////////////////////////
//                   CURSOR FUNCTIONS                         //
//            setPosition, setColor, setHidden                //
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

// get foreground printing color of cursor
// uses private variable rather than screen buffer
// this function doesn't return errors
int SWTI_Cursor::getColorForeground()
{
  return cForeground;
}

// get background printing color of cursor
// uses private variable rather than screen buffer
// this function doesn't return errors
int SWTI_Cursor::getColorBackground()
{
  return cBackground;
}

// set cursor position, example: moving a player around the screen
// set the position using handle and function in windows.h
// if the function fails, it returns false
bool SWTI_Cursor::setPosition(int x, int y)
{
  COORD point; point.X = x; point.Y = y;
  BOOL result = SetConsoleCursorPosition(hOutput, point);
  SWTI_PERR(result, "Cursor.setPosition", "SetConsoleCursorPosition");
  return SWTI_BOOL(result);
}

// set cursor color, print in different colors than white
// color is an enum defined in header file, e.x. SetCursorColor(RED);
// tip: you can also specify the background color
bool SWTI_Cursor::setColor(Color foreground, Color background)
{
  if (foreground == DEFAULT) foreground = cForeground;
  else cForeground = foreground;

  if (background == DEFAULT) background = cBackground;
  else cBackground = background;

  WORD wcol = foreground + 16 * background;
	BOOL result = SetConsoleTextAttribute(hOutput, wcol);
  SWTI_PERR(result, "Cursor.setColor", "SetConsoleTextAttribute");
  return SWTI_BOOL(result);
}

// print one char at specified position
// character can be any ascii character
// use cout for printing more characterss
bool SWTI_Cursor::printChar(int x, int y, int character, Color color)
{
  bool result = SWTI_Cursor::setPosition(x, y);
  SWTI_PERR(result, "Cursor.printChar", "Cursor.setPosition");
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
  bool setpos, ret;

  // get number of chars
  rgcsbi = GetConsoleScreenBufferInfo(hOutput, &csbi);
  dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

  // clear entire screen with blanks, set text atribute to current atribute
  rfcoc = FillConsoleOutputCharacter(hOutput,
    (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten);
  retgcsbi = GetConsoleScreenBufferInfo(hOutput, &csbi);   // set text atribute
  rfcoa = FillConsoleOutputAttribute(hOutput,
    csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);
  setpos = SWTI_Cursor::setPosition(0, 0);   // reset position

  // print any error
  SWTI_PERR(rgcsbi, "Cursor.clearScreen", "GetConsoleScreenBufferInfo");
  SWTI_PERR(rfcoc, "Cursor.clearScreen", "FillConsoleOutputCharacter");
  SWTI_PERR(retgcsbi, "Cursor.clearScreen", "GetConsoleScreenBufferInfo");
  SWTI_PERR(rfcoa, "Cursor.clearScreen", "FillConsoleOutputAttribute");
  SWTI_PERR(setpos, "Cursor.clearScreen", "Cursor.setPosition");

  ret = SWTI_BOOL(rgcsbi) && SWTI_BOOL(rgcsbi) && SWTI_BOOL(rgcsbi)
        && SWTI_BOOL(rgcsbi) && setpos;
  return ret;
}

// get cursor width in pixels, if it fails, it returns 0
// use font structucture and winapi font functions
int SWTI_Cursor::getFontWidth()
{
  CONSOLE_FONT_INFOEX font={0};
  font.cbSize = sizeof(font);
  BOOL result = GetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.getFontWidth", "GetCurrentConsoleFontEx");
  return result ? font.dwFontSize.X : SWTI_ERROR;
}

// get cursor height in pixels, if it fails, it returns 0
// use font structucture and winapi font functions
int SWTI_Cursor::getFontHeight()
{
  CONSOLE_FONT_INFOEX font={0};
  font.cbSize = sizeof(font);
  BOOL result = GetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.getFontHeight", "GetCurrentConsoleFontEx");
  return result ? font.dwFontSize.Y : SWTI_ERROR;
}

// set new height of console font
// set size proportionally to screen height
// width will be calculated automatically
bool SWTI_Cursor::setFontSize(int size)
{
  int wh = Window.getHeight();
  SWTI_PERRI(wh, "Cursor.setFontSize", "Window.getHeight");
  size = (wh * size)/1000;
  bool result = SWTI_Cursor::setFontPixels(0,size);
  SWTI_PERR(result, "Cursor.setFontSize", "Cursor.setFontPixels");
  return result;
}

// set cursor font size, good for export to different monitor sizes
// tip: if one argument is 0, it is automatically calculated
// use the font structure and extern C function
bool SWTI_Cursor::setFontPixels(int width, int height)
{
  int wwidth = Window.getWidth(); // save window width
  int wheight = Window.getHeight(); // save window height

  SWTI_PERRI(wwidth, "Cursor.setFontPixels", "Window.getWidth");
  SWTI_PERRI(wheight, "Cursor.setFontPixels", "Window.getHeight");

  CONSOLE_FONT_INFOEX font={0};
  font.cbSize = sizeof(font);
  font.dwFontSize.X = width;
  font.dwFontSize.Y = height;
  BOOL result = SetCurrentConsoleFontEx(hOutput, false, &font);
  SWTI_PERR(result, "Cursor.setFontPixels", "SetCurrentConsoleFontEx");

  Sleep(SWTI_DELAY);  // slow winapi function needs a delay
  bool ret = Window.setSizePixels(wwidth,wheight);  // reset size
  SWTI_PERR(ret,"Cursor.setFontPixels", "Window.setSizePixels")
  ret &= SWTI_BOOL(result); // ret is true only if it and result are true
  return ret;
}

// set font size based on specified characters in window
// good for export to different monitor sizes
// use the font structure and extern C function
bool SWTI_Cursor::setFontChars(int columns, int rows)
{
  int width = Window.getWidth();
  int height = Window.getHeight();
  bool rsfp = SWTI_Cursor::setFontPixels(width/columns,height/rows);
  bool rssp = Window.setSizePixels(width,height);

  SWTI_PERR(width,"Cursor.setFontChars","Window.getWidth");
  SWTI_PERR(height,"Cursor.setFontChars","Window.getHeight");
  SWTI_PERR(rsfp,"Cursor.setFontChars","Cursor.setFontPixels");
  SWTI_PERR(rssp,"Cursor.setFontChars","Window.setSizePixels");

  rsfp &= rssp;
  return rsfp;
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

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  BOOL result = GetConsoleScreenBufferInfo(hOutput, &csbi);
  SWTI_PERR(result, "Window.getColumns", "GetConsoleScreenBufferInfo");
  cForeground = (Color) (csbi.wAttributes % 16);
  cBackground = (Color) (csbi.wAttributes / 16);
  SWTI_Cursor::clearScreen();
}

// private destructor of cursor
// handles are closed by os
SWTI_Cursor::~SWTI_Cursor() { }


////////////////////////////////////////////////////////////////
//                   KEYBOARD FUNCTIONS                       //
//                get, getPressed, getReleased                //
////////////////////////////////////////////////////////////////

// test if key is currently down, e. x. try GetKey(VK_LEFT) or GetKey('A')
// using the number 0x8000 to get the most important bit
// this function does not return error value
bool SWTI_Keyboard::get(int key)
{
  return GetKeyState(key) & 0x8000;
}

// test if key was pressed since the last call
// can be used with Wait or Sleep commands and doesn't stop the program
// use custom keyboard storage for fast check
// this function does not return error value
bool SWTI_Keyboard::getPressed(int key)
{
  bool previous = bPressed[key];
  bPressed[key] = SWTI_Keyboard::get(key);
  return (!previous && bPressed[key]);
}

// test if key was released since the last call
// can be used with wait or Sleep commands and doesn't stop the program
// use custom keyboard storage for fast check
// this function does not return error value
bool SWTI_Keyboard::getReleased(int key)
{
  bool previous = bReleased[key];
  bReleased[key] = SWTI_Keyboard::get(key);
  return (previous && !bReleased[key]);
}

// pause application for ticks per second
// higher values means faster program
// good for slowing fast input checks
// returns true if console was paused
bool SWTI_Keyboard::wait(unsigned int ticks)
{
  bool wt = (ticks > 0);
  if (wt) Sleep(1000 / ticks);
  return wt;
}

// wait until user presses any key
// returns false if error occured
bool SWTI_Keyboard::waitUser()
{
  int size = 256, prev;
  BYTE keys[256]; // structure that holds pressed keys
  BOOL result;

  do {
    // one check is neccessary for GetKeyboardState to work
    if (SWTI_Keyboard::getPressed(VK_ESCAPE)) return true;

    // get the current pressed keys and slow the function
    result = GetKeyboardState(keys);
    SWTI_PERR(result,"Keyboard.waitUser","GetKeyboardState");
    if (!SWTI_BOOL(result)) return false;
    Sleep(SWTI_DELAY);

    // save the number of pressed keys
    prev = size;
    size = 0;

    // go through all the keys, check the most important bit
    for (int i = 0; i < 256; i++)
      if (keys[i] >> 7) size++;

    // end function if key was pressed
  } while(size <= prev);

  return true;
}

// get instance of keyboard
// only one object is needed
SWTI_Keyboard& SWTI_Keyboard::getInstance()
{
  static SWTI_Keyboard instance; // guaranteed to be destroyed
  return instance; // instantiated on first use
}

// private constructor of keyboard
// set all keys to zero
SWTI_Keyboard::SWTI_Keyboard()
{
  // keyboard has 256 unique keys
  // visual studio can't
  for(int i = 0; i < 256; i++)
  {
    bPressed[i] = 0;
    bReleased[i] = 0;
  }
}

// private destructor of keyboard
// all variables are destroyed automatically
SWTI_Keyboard::~SWTI_Keyboard() { }


////////////////////////////////////////////////////////////////
//                  MOUSE OPERATIONS                          //
//           GetMousePosX, GetMousePosY, GetMouseKey*         //
////////////////////////////////////////////////////////////////

// x position of mouse in console window in pixels
// can return negatives and numbers larger than screen width
// prints error message when error occurs
int SWTI_Mouse::getX()
{
  POINT pt;
  BOOL result = GetCursorPos(&pt);
  int bh = Window.getBarHeight();
  int x = Window.getX();

  SWTI_PERR(result, "Mouse.getX", "GetCursorPos");
  SWTI_PERRI(bh,"Mouse.getX","Window.getBarHeight");
  SWTI_PERRI(x,"Mouse.getX","Window.getX");
  if (x == SWTI_ERROR || result == SWTI_ERROR) return SWTI_ERROR;

  x = pt.x - x;
  if (bh) x -= 8;
  return x;
}

// y position of mouse in console window in pixels
// can return negatives and numbers larger than screen height
// prints error message when error occurs
int SWTI_Mouse::getY()
{
  POINT pt;
  BOOL result = GetCursorPos(&pt);
  int bh = Window.getBarHeight();
  int y = Window.getY();

  SWTI_PERR(result,"Mouse.getY","GetCursorPos");
  SWTI_PERRI(bh,"Mouse.getY","Window.getBarHeight");
  SWTI_PERRI(y,"Mouse.getY","Window.getY");

  if (y == SWTI_ERROR || bh == SWTI_ERROR || result == SWTI_ERROR)
    return SWTI_ERROR;

  y = pt.y - y;
  if (bh) y += 8;
  return y;
}

// x position of mouse in console window in columns
// when console font is unknown use 13 as default
// use for hovering or clicking on text in console
int SWTI_Mouse::getColumns()
{
  int x = SWTI_Mouse::getX();
  int cw = Cursor.getFontWidth();
  SWTI_PERRI(cw,"Mouse.getColumns","Cursor.getFontWidth");
  x = (cw != SWTI_ERROR) ? x/cw : x/13; // convert to columns
  return x;
}

// y position of mouse in console window in rows
// when console font is unknown use 24 as default
// use for hovering or clicking on text in console
int SWTI_Mouse::getRows()
{
  int y = SWTI_Mouse::getY();
  int ch = Cursor.getFontHeight();
  SWTI_PERRI(ch,"Mouse.getRows","Cursor.getFontHeight");
  y = (ch != SWTI_ERROR) ? y/ch : y/24; // convert to columns
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
// get size of topbar
SWTI_Mouse::SWTI_Mouse() { }

// private destructor of mouse
// all variables are destroyed automatically
SWTI_Mouse::~SWTI_Mouse() { }


////////////////////////////////////////////////////////////////
//                 WINDOW OPERATIONS                          //
//    setFullscreenWindow, SetWindowPosition, SetWindowSize   //
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
  SWTI_PERRI(bh,"Window.getY", "Window.getBarHeight");
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

// Returns pozitive number if window is in borderless fullscreen
// Returns size of title bar
int SWTI_Window::getBarHeight()
{
  BOOL rwr, rcr;
  RECT window, client;
  rwr = GetWindowRect(hWindow, &window);
  rcr = GetClientRect(hWindow, &client);
  SWTI_PERR(rwr, "Window.getBarHeight", "GetWindowRect");
  SWTI_PERR(rwr, "Window.getBarHeight", "GetClientRect");
  int size = (window.bottom - window.top) - client.bottom;
  return (rwr && rcr) ? size : SWTI_ERROR;
}

// get width of monitor in pixels
int SWTI_Window::getScreenWidth()
{
  return GetSystemMetrics(SM_CXSCREEN);
}

// get height of monitor in pixels
int SWTI_Window::getScreenHeight()
{
  return GetSystemMetrics(SM_CYSCREEN);
}

// Set window position in pixels
bool SWTI_Window::setPositionPixels(int x, int y)
{
  int width = SWTI_Window::getWidth();
  int height = SWTI_Window::getHeight();
  BOOL result = MoveWindow(hWindow, x, y, width, height, TRUE);

  SWTI_PERRI(width, "Window.setPositionPixels", "Window.getWidth");
  SWTI_PERRI(height, "Window.setPositionPixels", "Window.getHeight");
  SWTI_PERR(result, "Window.setPositionPixels", "MoveWindow");

  bool ret = SWTI_BOOL(result);
  ret &= (width != SWTI_ERROR) & (height != SWTI_ERROR);
  return ret;
}

// Set window size in pixels
// Set position to center of window
bool SWTI_Window::setSizePixels(int width, int height)
{
  int newx = SWTI_Window::getScreenWidth()/2 - width/2;
  int newy = SWTI_Window::getScreenHeight()/2 - height/2;
  BOOL result = MoveWindow(hWindow, newx, newy, width, height, TRUE);
  bool ret; // return value

  SWTI_PERRI(newx, "Window.setSizePixels", "getScreenWidth");
  SWTI_PERRI(newy, "Window.setSizePixels", "getScreenHeight");
  SWTI_PERR(result, "Window.setSizePixels", "MoveWindow");

  ret = SWTI_BOOL(result) && (newx != SWTI_ERROR) && (newy != SWTI_ERROR);
  return ret;
}


// set the size of console window in Columns and Rows
// if you want fullscreen, use setFullscreenWindow() instead
// return true if resize was successful
bool SWTI_Window::setSizeChars(int columns, int rows)
{
  COORD coord;
  coord.X = columns;
  coord.Y = rows;

  SMALL_RECT wsize;
  wsize.Left = 0;
  wsize.Top = 0;
  wsize.Right = columns - 1;
  wsize.Bottom = rows - 1;

  BOOL rscsbi,rscwi;
  rscsbi = SetConsoleScreenBufferSize(hOutput, coord);
  rscwi = SetConsoleWindowInfo(hOutput, TRUE, &wsize);

  SWTI_PERR(rscsbi, "Window.setSizeChars", "SetConsoleScreenBufferSize");
  SWTI_PERR(rscwi,  "Window.setSizeChars", "SetConsoleWindowInfo");

  Sleep(SWTI_DELAY);

  // move window to center
  int width = SWTI_Window::getWidth();
  int height = SWTI_Window::getHeight();
  bool result = SWTI_Window::setSizePixels(width,height); // set position to center

  SWTI_PERRI(width,  "Window.setSizeChars", "Window.getWidth");
  SWTI_PERRI(height, "Window.setSizeChars", "Window.getHeight");
  SWTI_PERR(result,  "Window.setSizeChars", "Window.setSizePixels");

  return SWTI_BOOL(rscsbi) && SWTI_BOOL(rscwi);
}

// set the size of console window to maximum size
// warning: this function resets the cursor visibility
bool SWTI_Window::setFullscreenWindow()
{
  int bh = SWTI_Window::getBarHeight();
  SWTI_PERRI(bh,"Window.setFullscreenWindow", "Window.getBarHeight");

  if (bh == 0) // if window is in borderless mode
  {
    bool res = SWTI_Window::setFullscreenBorderless(); // press f11
    SWTI_PERR(res, "Window.setFullscreenWindow",
      "Window.setFullscreenBorderless");
  }

  // set size to maximum window
  COORD maxsize;
  bool result, wspp, wssc;

  maxsize = GetLargestConsoleWindowSize(hOutput);
  wspp = SWTI_Window::setPositionPixels(0, 0);
  wssc = SWTI_Window::setSizeChars(maxsize.X, maxsize.Y);

  if (maxsize.X == 0 && maxsize.Y == 0) SWTI_PERR(FALSE,
    "Window.setFullscreenWindow", "GetLargestConsoleWindowSize");
  SWTI_PERR(wspp, "Window.setFullscreenWindow", "Window.setPositionPixels");
  SWTI_PERR(wspp, "Window.setFullscreenWindow", "Window.setSizeChars");

  result = wspp && wssc && (maxsize.X != 0 || maxsize.Y != 0);
  return result;
}

// set the size of console window to maximum size without borders
// warning: this function resets the cursor visibility
bool SWTI_Window::setFullscreenBorderless()
{
  int bh = SWTI_Window::getBarHeight();
  SWTI_PERRI(bh,"Window.setFullscreenBorderless", "Window.getBarHeight");

  if (bh > 0) // if window is not in borderless mode
  {
    INPUT ip;
    BOOL retpress, retrelease;
    bool result;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0; // hardware scan code for key
    ip.ki.time = 0; // time in ms
    ip.ki.dwExtraInfo = 0; // no extra info
    ip.ki.wVk = VK_F11; // virtual-key code for the VK_F11 key

    ip.ki.dwFlags = 0; // 0 for key press
    retpress = SendInput(1, &ip, sizeof(INPUT)); // Press the F11 key
    SWTI_PERR(retpress, "Window.setFullscreenWindow", "SendInput");

    ip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    retrelease = SendInput(1, &ip, sizeof(INPUT)); // Release the F11 key
    SWTI_PERR(retrelease, "Window.setFullscreenWindow", "SendInput");

    Sleep(SWTI_DELAY); // wait for apply
    result = SWTI_Window::hideScrollbars(); // hide scrollbars
    SWTI_PERR(result, "Window.setFullscreenWindow", "Window.hideScrollbars");
    return (retpress && retrelease);
  }
  return true;
}

// set colors for the whole window
// changes all current colors and cursor colors
bool SWTI_Window::setColor(Color foreground, Color background)
{
  if (foreground == DEFAULT) foreground = (Color) Cursor.getColorForeground();
  if (background == DEFAULT) background = (Color) Cursor.getColorBackground();

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
  result = Cursor.setColor(foreground, background); // set default cursor

  // print any error
  SWTI_PERR(rgcsbi, "Window.setColor", "GetConsoleScreenBufferInfo");
  SWTI_PERR(rfcoa,  "Window.setColor", "FillConsoleOutputAttribute");
  SWTI_PERR(result, "Window.setColor", "Cursor.setColor");

  // return result
  result = result && SWTI_BOOL(rgcsbi) && SWTI_BOOL(rfcoa);
  return result;
}


// set the visibility of cursor as visible
// show the blinking cursor when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::showBlinking()
{
  // set blinking cursor
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 25;  // cursor size isn't the font size
  info.bVisible = TRUE;  // bVisible is an int
  BOOL result = SetConsoleCursorInfo(hOutput, &info);
  SWTI_PERR(result, "Window.showBlinking", "SetConsoleCursorInfo");
  return SWTI_BOOL(result); // return true if everything is alright
}

// set the visibility of cursor as hidden
// hide the blinking cursor when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::hideBlinking()
{
  // set blinking cursor
  CONSOLE_CURSOR_INFO info;
  info.dwSize = 25;  // cursor size isn't the font size
  info.bVisible = FALSE;  // bVisible is an int
  BOOL result = SetConsoleCursorInfo(hOutput, &info);
  SWTI_PERR(result, "Window.hideBlinking", "SetConsoleCursorInfo");
  return SWTI_BOOL(result); // return true if everything is alright
}

// set selection visibility
// show selection when printing and scanning
// use this function AFTER resizing a window else it won't work
bool SWTI_Window::showSelection()
{
  DWORD mode; // holds all the console flags
  BOOL rgcm; // store messages from winapi
  BOOL rscm; // store messages from winapi
  bool result; // return value

  // get console information and enable edit mode
  rgcm = GetConsoleMode(hInput, &mode);
  rscm = SetConsoleMode(hInput, mode & ENABLE_QUICK_EDIT_MODE);

  // print any error
  SWTI_PERR(rgcm, "Window.showSelection", "GetConsoleMode");
  SWTI_PERR(rscm, "Window.showSelection", "SetConsoleMode");

  result = SWTI_BOOL(rgcm) & SWTI_BOOL(rscm);
  return result;
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

  result = SWTI_BOOL(rgcm) & SWTI_BOOL(rscm);
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

  ret = SWTI_BOOL(result) && (wgc != SWTI_ERROR) && (wgr != SWTI_ERROR);
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
  LPCWSTR lcp = (LPCWSTR) title.c_str();
  BOOL result = SetConsoleTitle(lcp);
  SWTI_PERR(result, "Window.setTitle", "SetConsoleTitle");
  return SWTI_BOOL(result);
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
