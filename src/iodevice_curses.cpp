#include "iodevice_curses.hpp"

#include <curses.h>
#include <string>

char IODeviceCurses::getChar() {
    return getch();
}

void IODeviceCurses::setColor(int colorCode) {
    color_set(colorCode, 0);
}

void IODeviceCurses::putString(int x, int y, std::string to_print) {
    mvaddstr(y, x, to_print.c_str());
}

void IODeviceCurses::moveCursor(int x, int y) {
    move(y, x) ;
}

void IODeviceCurses::setCursorVisibility(int visibility) {
    curs_set(visibility);
}

int IODeviceCurses::getHeight() {
    return LINES;
}

int IODeviceCurses::getWidth() {
    return COLS;
}

void IODeviceCurses::clear() {
    clear();
}

void IODeviceCurses::refresh() {
    refresh();
}

void IODeviceCurses::initWindow() {
    initscr();
}

void IODeviceCurses::endWindow() {
    endwin();
}

void IODeviceCurses::setEcho(bool enabled) {
    if (enabled) {
        echo();
    } else {
        noecho();
    }
}

void IODeviceCurses::startColor() {
    start_color();
}

void IODeviceCurses::startSpecialKeys() {
    keypad(stdscr, TRUE);
    cbreak();
}

void IODeviceCurses::addColor(int id, int fg, int bg) {
    init_pair(id, fg, bg);
}

void IODeviceCurses::setBackground(int id) {
    bkgd(COLOR_PAIR(id));
}
