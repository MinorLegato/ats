#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#define WIN32_LEAN_AND_MEAN

#include <windows.h>

// =================================== WIN32 CONSOLE FUNCTIONS ================================== //

#define KEY_PRESS ((uint16_t)(0x8000)) 

enum {
    COLOR_BLACK             = 0,
    COLOR_DARKBLUE          = FOREGROUND_BLUE,
    COLOR_DARKGREEN         = FOREGROUND_GREEN,
    COLOR_DARKCYAN          = FOREGROUND_GREEN | FOREGROUND_BLUE,
    COLOR_DARKRED           = FOREGROUND_RED,
    COLOR_DARKMAGENTA       = FOREGROUND_RED | FOREGROUND_BLUE,
    COLOR_DARKYELLOW        = FOREGROUND_RED | FOREGROUND_GREEN,
    COLOR_DARKGRAY          = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
    COLOR_GRAY              = FOREGROUND_INTENSITY,
    COLOR_BLUE              = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
    COLOR_GREEN             = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
    COLOR_CYAN              = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
    COLOR_RED               = FOREGROUND_INTENSITY | FOREGROUND_RED,
    COLOR_MAGENTA           = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
    COLOR_YELLOW            = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
    COLOR_WHITE             = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
};

#define MAKE_COLOR(_FORE, _BACK) ((int)((_FORE) | ((_BACK) << 4)))

static HANDLE console_output;

static int consoleGetFontSize(COORD *size) {
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };
 
    if (!GetCurrentConsoleFontEx(console_output, 0, &font))
        return 0;
 
    *size = font.dwFontSize;
 
    return 1;
}

static int consoleSetFontSize(COORD size) {
    CONSOLE_FONT_INFOEX font = { sizeof(CONSOLE_FONT_INFOEX) };
 
    if (!GetCurrentConsoleFontEx(console_output, 0, &font))
        return 0;
 
    font.dwFontSize = size;
 
    if (!SetCurrentConsoleFontEx(console_output, 0, &font))
        return 0;
    
    return 1;

}

static void consoleInit(int font_x, int font_y) {
    system("MODE 180, 60");
    system("cls");
    system("TITLE CONSOLE RPG");

    console_output = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD font_size = { font_x, font_y };

    consoleSetFontSize(font_size);
}

static void consoleSetCursor(int x, int y) {
    COORD pos = { x, y };
    SetConsoleCursorPosition(console_output, pos);
}

static void consoleSetColor(int foreground, int background) {
    SetConsoleTextAttribute(console_output, MAKE_COLOR(foreground, background));
}

void consoleHideCursor(void) {
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(console_output, &info);
}

#endif

