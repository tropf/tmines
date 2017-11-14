#include <inputdevice_curses.hpp>
#include <curses.h>

char InputdeviceCurses::getChar() {
    return getch();
}
