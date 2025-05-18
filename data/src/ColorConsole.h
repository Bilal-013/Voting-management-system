#ifndef COLOR_CONSOLE_H
#define COLOR_CONSOLE_H

#include <windows.h>
#include <string>
#include <iostream>

// Bring iostream symbols into scope
using std::cout;
using std::endl;
using std::string;

class ColorConsole {
public:
    enum class Color {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        LIGHTGRAY = 7,
        DARKGRAY = 8,
        LIGHTBLUE = 9,
        LIGHTGREEN = 10,
        LIGHTCYAN = 11,
        LIGHTRED = 12,
        LIGHTMAGENTA = 13,
        YELLOW = 14,
        WHITE = 15
    };

    // Set text color
    static void setTextColor(Color color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
    }

    // Reset text color to default (white)
    static void resetColor() {
        setTextColor(Color::WHITE);
    }

    // Print colored text and reset color
    static void printColored(const std::string& text, Color color) {
        setTextColor(color);
        std::cout << text;
        resetColor();
    }

    // Center and print colored text
    static void centerColoredText(const std::string& text, Color color) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        
        int padding = (consoleWidth - text.length()) / 2;
        std::cout << std::string(padding, ' ');
        printColored(text, color);
        std::cout << std::endl;
    }
    
    // Print a colored divider line
    static void coloredDivider(char symbol = '-', Color color = Color::CYAN) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        
        setTextColor(color);
        std::cout << std::string(consoleWidth, symbol);
        resetColor();
        std::cout << std::endl;
    }
};

#endif // COLOR_CONSOLE_H
