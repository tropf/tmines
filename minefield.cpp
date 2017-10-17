#include "minefield.hpp"

#include <vector>
#include <stdexcept>

#include "doctest.h"
        
bool Minefield::isMine(int x, int y) {

}

Minefield::Minefield(int dimension_x, int dimension_y) {
    if (dimension_x <= 0 || dimension_y <= 0) {
        throw std::range_error("Given X and Y dimensions must be >0.");
    }

    for (int x = 0; x < dimension_x; x++) {
        std::vector<bool> column;
        for (int y = 0; y < dimension_y; y++) {
            column.push_back(false);
        }
        mines.push_back(column);
        flags.push_back(column);
    }
}

bool Minefield::isGameEnded() {

}

bool Minefield::isGameRunning() {

}

bool Minefield::isGameWon() {

}

bool Minefield::isGameLost() {

}

bool Minefield::isPosValid(int x, int y) {
    if (x < 0) {
        return false;
    }
    if (x >= getXDimension()) {
        return false;
    }

    if (y < 0) {
        return false;
    }
    if (y >= getYDimension()) {
        return false;
    }

    return true;
}

bool Minefield::isFlagged(int x, int y) {

}

void Minefield::flag(int x, int y) {

}

void Minefield::open(int x, int y) {

}

int Minefield::getSorroundingMineCount(int x, int y) {

}

int Minefield::getXDimension() {
    return mines.size();
}

int Minefield::getYDimension() {
    return mines[0].size();
}
