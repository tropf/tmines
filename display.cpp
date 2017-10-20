#include "display.hpp"

#include "controller.hpp"

#include <iostream>
#include <curses.h>
#include <string>

void Display::renderBoard() {
    auto mfield = controller.getMinefield();
    
    for (int x = 0; x < mfield.getXDimension(); x++) {
        for (int y = 0; y < mfield.getYDimension(); y++) {
            char to_print;
            if (mfield.isOpen(x, y)) {
                to_print = std::to_string(mfield.getSorroundingMineCount(x, y)).c_str()[0];
                if ('0' == to_print) {
                    to_print = ' ';
                }

                if (mfield.isMine(x, y)) {
                    to_print = 'X';
                }
            } else if (mfield.isFlagged(x, y)) {
                to_print = '?';
            } else {
                to_print = '*';
            }

            int x_to_print, y_to_print;
            std::tie(x_to_print, y_to_print) = getConsolePosition(x, y);
            mvinsch(y_to_print, x_to_print, to_print);
        }
    }
}

std::tuple<int, int> Display::getConsolePosition(int x, int y) {
    return std::make_tuple(1 + x * 2, y);
}

void Display::renderStatusline() {

}

int Display::getKey() {
    return getch();
}

void Display::handleKey(int key) {
    std::string s = std::to_string(key);

    mvprintw(3, 5, s.c_str());

    if (113 == key) {
        exit = true;
    } else if (32 == key) {
        controller.click();
    } else if (119 == key) {
        controller.moveUp();
    } else if (97 == key) {
        controller.moveLeft();
    } else if (115 == key) {
        controller.moveDown();
    } else if (100 == key) {
        controller.moveRight();
    } else if (102 == key) {
        controller.tooggleFlag();
    }
}

void Display::updateCursor() {
    int x, y;
    std::tie(x, y) = getConsolePosition(controller.getX(), controller.getY());
    move(y, x);
}

void Display::run() {
    while(!exit) {
        // rendering process
        clear();
        curs_set(0);
        renderBoard();
        renderStatusline();
        updateCursor();
        curs_set(1);
        handleKey(getKey());
    }
}

Display::Display(int width, int height, int mine_count, int seed) {
    controller = Controller(width, height, mine_count, seed);
    exit = false;

    initscr();
    noecho();
    curs_set(2);

    run();

    endwin();
}

