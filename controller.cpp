#include "controller.hpp"

#include <iostream>

Controller::Controller(int width, int height, int mine_count, int seed) {
    x = 0;
    y = 0;

    mfield = Minefield(width, height, mine_count, seed);
}

int Controller::getX() {
    return x;
}

int Controller::getY() {
    return y;
}

int Controller::getWidth() {
    return mfield.getXDimension();
}

int Controller::getHeight() {
    return mfield.getYDimension();
}

void Controller::moveRight() {
    putCursor(x + 1, y);
}

void Controller::moveLeft() {
    putCursor(x - 1, y);
}

void Controller::moveUp() {
    putCursor(x, y - 1);
}

void Controller::moveDown() {
    putCursor(x, y + 1);
}

void Controller::putCursor(int given_x, int given_y) {
    if (mfield.isPosValid(given_x, given_y)) {
        x = given_x;
        y = given_y;
    }
}

void Controller::click() {
    click(x, y);
}

void Controller::click(int given_x, int given_y) {
    // check pos
    if (mfield.isPosValid(given_x, given_y)) {
        // only do stuff while game is still running
        if (mfield.isGameRunning()) {
            // only click on fields w/o flags
            if(! mfield.isFlagged(given_x, given_y)) {
                mfield.open(given_x, given_y);
            }
        }
    }
}

void Controller::tooggleFlag() {
    tooggleFlag(x, y);
}

void Controller::tooggleFlag(int given_x, int given_y) {
    // check pos
    if (mfield.isPosValid(given_x, given_y)) {
        // only do stuff while game is running
        if (mfield.isGameRunning()) {
            // don't do anything on already open fields
            if (! mfield.isOpen(given_x, given_y)) {
                // no choose: flag or unflag
                if (mfield.isFlagged(given_x, given_y)) {
                    mfield.unflag(given_x, given_y);
                } else {
                    mfield.flag(given_x, given_y);
                }
            }
        }
    }
}

void Controller::print() {
    for (int current_y = 0; current_y < mfield.getYDimension(); current_y++) {
        for (int current_x = 0; current_x < mfield.getXDimension(); current_x++) {
            if ((x == current_x) && (y == current_y)) {
                std::cout << "[";
            } else {
                std::cout << " ";
            }

            if (mfield.isFlagged(current_x, current_y)) {
                std::cout << "?";
            } else if (! mfield.isOpen(current_x, current_y)) {
                std::cout << "*";
            } else if (mfield.isMine(current_x, current_y)) {
                std::cout << "X";
            } else if (0 == mfield.getSorroundingMineCount(current_x, current_y)) {
                std::cout << " ";
            } else {
                std::cout << mfield.getSorroundingMineCount(current_x, current_y);
            }

            if (x == current_x && y == current_y) {
                std::cout << "]";
            } else {
                std::cout << " ";
            }
        }
        std::cout << std::endl;
    }
}

Minefield Controller::getMinefield() {
    return mfield;
}

