#include "iodevice_curses.hpp"

#include <curses.h>

char IODeviceCurses::getChar() {
    return getch();
}
