#ifndef SWTI_HEADER
#define SWTI_HEADER

////////////////////////////////////////////////////////////////
//                                                            //
//                SIMPLE WINDOWS TEXT INTERFACE               //
//             better interaction with console window         //
//                                                            //
//         Objects:                                           //
//            Cursor - for writing text                       //
//            Keyboard - for getting key input                //
//            Mouse - for getting mouse position              //
//            Window - for setting console window             //
//                                                            //
//         Basic Info:                                        //
//            Pixel - one point on screen with one color      //
//            Character - one character in console window     //
//            Column - x position of character                //
//            Row - y position of character                   //
//                                                            //
////////////////////////////////////////////////////////////////

#include <iostream> // printing and string
#include <windows.h> // windows console library

// all colors for cursor and window, CURRENT color is the one currently in use
// change cursor color using Cursor.setColor and window colors Window.setColor
enum Color {BLACK, BLUE, GREEN, CYAN, RED, MAGENTA, YELLOW, LIGHTGRAY,  GRAY,
  LIGHTBLUE, LIGHTGREEN, LIGHTCYAN, LIGHTRED, LIGHTMAGENTA, LIGHTYELLOW, WHITE, CURRENT};

////////////////////////////////////////////////////////////////
//                       CURSOR OBJECT                        //
//              setPosition, setColor, printChar              //
//          cursor writes in console window all text          //
////////////////////////////////////////////////////////////////

class SWTI_Cursor {
public:
  // get and set position
  int getX(); // x position in columns
  int getY(); // y position in rows
  Color getColorForeground(); // get foreground printing color
  Color getColorBackground(); // get background printing color
  bool setPosition(int x, int y); // set printing position
  bool setColor(Color foreground, Color background=CURRENT); // set printing color
  bool printChar(int x, int y, int character, Color color=CURRENT); // cout character
  bool printBlank(int x, int y);  // print blank character on position
  bool clearScreen(); // clears whole console window, slow function

  // font operations
  int getFontWidth();  // font width in pixels
  int getFontHeight(); // font height in pixels
  int getFontSize(); // font size proportional to window size
  std::wstring getFontType(); // get font name
  bool setFontSize(int size); // set font size proportionally to window size
  bool setFontChars(int columns, int rows); // adjust font size for columns and rows
  bool setFontPixels(int width, int height); // set font size, default (13,24)
  bool setFontType(const std::wstring name); // set font type using font name

  //creating and deleting the cursor is automatic
  static SWTI_Cursor& getInstance(); // get the only one instance of cursor
  ~SWTI_Cursor(); // destructor is called automatically

private:
  HANDLE hOutput; // getting and setting the position
  SWTI_Cursor(); // private constructor for singleton pattern
};


////////////////////////////////////////////////////////////////
//                      KEYBOARD OBJECT                       //
//                get, getPressed, getReleased                //
//              check keys of keyboard or mouse               //
////////////////////////////////////////////////////////////////


class SWTI_Keyboard {
  // Keyboard keys are encoded as characters 'A' or virtual key codes VK_LEFT
  // Mouse keys are encoded as VK_LBUTTON, VK_RBUTTON, VK_MBUTTON
  // https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
public:
  bool get(int key); // is currently a key pressed
  bool getPressed(int key); // was a key pressed since last call
  bool getReleased(int key); // was a key released since last call
  bool wait(unsigned int ticks); // slow program to ticks per second
  bool waitUser(); // wait until user presses any key

  // creating and deleting the keyboard is automatic
  static SWTI_Keyboard& getInstance(); // get the only one instance of keyboard
  ~SWTI_Keyboard(); // destructor is called automatically

private:
  bool nKeys[256]; // boolean array that holds pressed keys
  bool pKeys[256]; // boolean array that holds released keys
  SWTI_Keyboard(); // private constructor for singleton pattern
};


////////////////////////////////////////////////////////////////
//                        MOUSE OBJECT                        //
//                    getColumns, getRows                     //
//                  check position of mouse                   //
////////////////////////////////////////////////////////////////

class SWTI_Mouse {
  // Mouse Position checking
  // Mouse Buttons are checked with the Keyboard
public:
  int getX(); // x position in pixels
  int getY(); // y position in pixels
  int getColumns(); // x position in columns
  int getRows(); // y position in rows

  // creating and deleting the mouse is automatic
  static SWTI_Mouse& getInstance(); // get the only one instance of mouse
  ~SWTI_Mouse(); // destructor is called automatically

private:
  SWTI_Mouse(); // private constructor for singleton pattern
};


////////////////////////////////////////////////////////////////
//                       WINDOW OBJECT                        //
//        setFullscreenBorderless, getColumns, getRows        //
//          set size and position of console window           //
////////////////////////////////////////////////////////////////

class SWTI_Window {
public:
  // get information about the window
  int getX(); // x position of top left corner in pixels
  int getY(); // y position of top left corner in pixels
  int getWidth(); // width in pixels
  int getHeight(); // height in pixels
  int getColumns(); // width in columns
  int getRows();    // height in rows
  int getBarHeight(); // height of title bar
  int getScreenWidth(); // screen width in pixels
  int getScreenHeight(); // screen height in pixels
  std::wstring getTitle(); // get console title
  int getDefaultColor(Color color); // get color table color in hex format

  // set size and position of the window
  bool setSizeChars(int columns, int rows);  // set size in columns and rows
  bool setSizePixels(int width, int height); // set size in pixels, center window
  bool setPositionPixels(int x, int y); // set position in pixels
  bool setPositionCenter(); // set position to center of the screen
  bool setFullscreenWindow(); // set window to window fullscreen mode
  bool setFullscreenBorderless(); // set window to fullscreen borderless mode
  bool setColor(Color foreground, Color background);  // fill screen with color
  bool setDefaultColor(Color color, int value); // set default color table
  bool setTitle(const std::wstring title); // set title of window

  // show and hide elements of the window
  bool showBlinking(); // show blinking cursor
  bool hideBlinking(); // hide blinking cursor
  bool showSelection(); // show cursor selection
  bool hideSelection(); // hide cursor selection
  bool showScrollbars(int columns, int rows); // show scrollbars
  bool hideScrollbars(); // hide right and bottom scrollbar
  bool hideResize(); // hide resize in window borders and navigation
  bool showResize(); // show resize in window borders and navigation

  // creating and deleting the window is automatic
  static SWTI_Window& getInstance(); // get the only one instance of window
  ~SWTI_Window(); // destructor is called automatically

private:
  HANDLE hOutput; // used when getting size in columns
  HANDLE hInput; // used when setting visibility
  HWND hWindow; // used when resizing
  SWTI_Window(); // private constructor for singleton pattern
};

// Character constants
enum ASCII {
  // Box drawing characters single line
  LINE_H = 196, LINE_V = 179, LINE_UR = 218, LINE_UL = 191,
  LINE_DR = 192, LINE_DL = 217, LINE_HU = 193, LINE_HD = 194,
  LINE_VR = 195, LINE_VL = 180, LINE_HV = 197,
  // Box drawing characters double line
  DLINE_H = 205, DLINE_V = 186, DLINE_UR = 201, DLINE_UL = 187,
  DLINE_DR = 200, DLINE_DL = 188, DLINE_HU = 202, DLINE_HD = 203,
  DLINE_VR = 204, DLINE_VL = 185,  DLINE_HV = 206,
  //  Graphic characters
  FILL_B = 32, FILL_L = 176, FILL_M = 177, FILL_H = 178, FILL_F = 219,
  SQUARE_D = 220, SQUARE_U = 223, SQUARE_M = 254,
  ARROWS_L = 174, ARROWS_R = 175
};

// function for converting hex color to windows color
// mainly used for setDefaultColor
int HEX(int number);

////////////////////////////////////////////////////////////////
//                     CREATE ALL OBJECTS                     //
//              Cursor, Keyboard, Window, Mouse               //
////////////////////////////////////////////////////////////////

extern SWTI_Window& Window;
extern SWTI_Cursor& Cursor;
extern SWTI_Keyboard& Keyboard;
extern SWTI_Mouse& Mouse;

#endif // end of include guard: SWTI_HEADER
