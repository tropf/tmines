#ifndef __IODEVICE_CURSES_H_INCLUDED__
#define __IODEVICE_CURSES_H_INCLUDED__

#include "iodevice.hpp"

#include <iostream>

class IODeviceCurses: public IODevice {
    public:
        char getChar();
        void setColor(int colorCode);
        void putString(int x, int y, std::string to_print);
        void moveCursor(int x, int y);
        void setCursorVisibility(int visibility);
        int getHeight();
        int getWidth();
        void clear();
        void refresh();
        void initWindow();
        void endWindow();
        void setEcho(bool enabled);
        void startColor();
        void startSpecialKeys();
        void addColor(int id, int fg, int bg);
        void setBackground(int id);
};

#endif // __IODEVICE_CURSES_H_INCLUDED__
