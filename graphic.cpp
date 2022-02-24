#include "graphic.h"
#include <iostream>

//CONSOLE PRESETTINGS
Console::Console() {
    //turn off underscore cursor
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(_hConsole, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(_hConsole, &cursorInfo);


    //...
}

//https://docs.microsoft.com/en-us/windows/console/clearing-the-screen?redirectedfrom=MSDN
void Console::clear() {
    COORD coordScreen = { 0, 0 };    // home for the cursor
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize;

    // Get the number of character cells in the current buffer.
    if (!GetConsoleScreenBufferInfo(_hConsole, &csbi)) { return; }

    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    // Fill the entire screen with blanks.
    if (!FillConsoleOutputCharacter(_hConsole,        // Handle to console screen buffer
        (TCHAR)' ',      // Character to write to the buffer
        dwConSize,       // Number of cells to write
        coordScreen,     // Coordinates of first cell
        &cCharsWritten)) // Receive number of characters written
    {
        return;
    }

    // Get the current text attribute.
    if (!GetConsoleScreenBufferInfo(_hConsole, &csbi)) {
        return;
    }

    // Set the buffer's attributes accordingly.
    if (!FillConsoleOutputAttribute(_hConsole,         // Handle to console screen buffer
        csbi.wAttributes, // Character attributes to use
        dwConSize,        // Number of cells to set attribute
        coordScreen,      // Coordinates of first cell
        &cCharsWritten))  // Receive number of characters written
    {
        return;
    }

    // Put the cursor at its home coordinates.
    SetConsoleCursorPosition(_hConsole, coordScreen);
}

void Console::setCursorPosition(int x, int y) {
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(_hConsole, coord);
}
void Console::update() {
    int colwriten = 0;
    for (size_t row = 0; row < buffer.size(); row++) {
        for (size_t col = 0; col < buffer[row].size(); col++) {
            setColor(buffer[row][col].color);
            setCursorPosition(colwriten, row);
            std::cout << buffer[row][col].content;
            colwriten += buffer[row][col].content.length();
        }
        colwriten = 0;
    }
    std::cout.flush();
    buffer.clear();
}

void Console::setColor(Color c) {
    SetConsoleTextAttribute(_hConsole, c);
}