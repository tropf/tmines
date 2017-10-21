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
                auto sourrounding_mine_count = mfield.getSorroundingMineCount(x, y);
                color_set(sourrounding_mine_count, 0);
                to_print = std::to_string(sourrounding_mine_count).c_str()[0];
                if ('0' == to_print) {
                    to_print = ' ';
                }

                if (mfield.isMine(x, y)) {
                    color_set(11, 0);
                    to_print = 'X';
                }
            } else if (mfield.isFlagged(x, y)) {
                color_set(12, 0);
                to_print = '?';
            } else {
                color_set(10, 0);
                to_print = '*';
            }

            int x_to_print, y_to_print;
            std::tie(x_to_print, y_to_print) = getConsolePosition(x, y);
            mvaddch(y_to_print, x_to_print, to_print);
        }
    }
}

std::tuple<int, int> Display::getConsolePosition(int x, int y) {
    return std::make_tuple(1 + x * 2, y);
}

void Display::renderStatusline() {
    std::string remaining_mines;
    std::string game_state;
    short color_to_use = 0;

    remaining_mines = "Verbleibende Minen: ";
    auto mfield = controller.getMinefield();
    if (mfield.isGameWon() || mfield.getFlagCount() > mfield.getMineCount()) {
        remaining_mines += "0";
    } else {
        remaining_mines += std::to_string(mfield.getMineCount() - mfield.getFlagCount());
    }
    if (mfield.isGameRunning()) {
        color_to_use = 12;
        game_state = "Spiel läuft";
    } else {
        if (mfield.isGameWon()) {
            color_to_use = 13;
            game_state = "Gewonnen";
        } else {
            color_to_use = 11;
            game_state = "Verloren";
        }
    }

    int x, y;
    std::tie(x, y) = getConsolePosition(0, mfield.getYDimension() + 1);

    color_set(0, 0);
    mvaddstr(y, x, "                          ");

    color_set(color_to_use, 0);
    mvaddstr(y, x, game_state.c_str());

    std::tie(x, y) = getConsolePosition(0, mfield.getYDimension() + 2);
    color_set(0, 0);
    mvaddstr(y, x, "                          ");
    color_set(12, 0);
    mvaddstr(y, x, remaining_mines.c_str());
}

int Display::getKey() {
    return getch();
}

void Display::handleKey(int key) {
    if (113 == key) {
        exit = true;
    } else if (' ' == key) {
        controller.click();
    } else if ('w' == key || KEY_UP == key || 'W' == key || 'k' == key || 'K' == key) {
        controller.moveUp();
    } else if ('a' == key || KEY_LEFT == key || 'A' == key || 'h' == key || 'H' == key) {
        controller.moveLeft();
    } else if ('s' == key || KEY_DOWN == key || 'S' == key || 'j' == key || 'J' == key) {
        controller.moveDown();
    } else if ('d' == key || KEY_RIGHT == key || 'D' == key || 'l' == key || 'L' == key) {
        controller.moveRight();
    } else if ('f' == key || 'F' == key) {
        controller.tooggleFlag();
    }
}

void Display::updateCursor() {
    int x, y;
    std::tie(x, y) = getConsolePosition(controller.getX(), controller.getY());
    move(y, x);
}

void Display::run() {
    init_pair(0, COLOR_WHITE, COLOR_BLACK);
    init_pair(1, COLOR_BLUE, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_CYAN, COLOR_BLACK);
    init_pair(5, COLOR_RED, COLOR_BLACK);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(8, COLOR_CYAN, COLOR_BLACK);
    init_pair(10, COLOR_WHITE, COLOR_BLACK);
    init_pair(11, COLOR_WHITE, COLOR_RED);
    init_pair(12, COLOR_YELLOW, COLOR_BLACK);
    init_pair(13, COLOR_GREEN, COLOR_BLACK);


    bkgd(COLOR_PAIR(10));

    while(!exit) {
        // rendering process
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
    start_color();
    keypad(stdscr, TRUE);
    cbreak();

    run();

    endwin();
}

