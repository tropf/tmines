#include "minefield.hpp"

#include <vector>
#include <stdexcept>
#include <tuple>
#include <random>
#include <iostream>
#include <algorithm>
#include <functional>

Minefield::Minefield(int dimension_x, int dimension_y, int mine_count, int seed) {
    if (dimension_x <= 0 || dimension_y <= 0) {
        throw std::range_error("Given X and Y dimensions must be >0.");
    }

    if (mine_count > dimension_x * dimension_y) {
        throw std::runtime_error("Given minecount doesn't fit on given X and Y dimensions");
    }

    if (mine_count < 0) {
        throw std::runtime_error("Given mine count can't be negative.");
    }

    // error checks ok, save params
    given_seed = seed;
    given_mine_count = mine_count;
    given_x_dimension = dimension_x;
    given_y_dimension = dimension_y;
    
    // init matrixes
    for (int x = 0; x < dimension_x; x++) {
        std::vector<bool> column;
        for (int y = 0; y < dimension_y; y++) {
            column.push_back(false);
        }
        mines.push_back(column);
        flags.push_back(column);
        opened.push_back(column);
    }

    // seed randomizer
    //A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
    std::mt19937 rdm_num_machine(seed);

    // pick mine_count random spots to place mines
    // 1. put all spots in list (encoded as x_dimension * y + x)
    // 2. pick random spot in list
    // 3. swap: first spot in list and chosen spot
    // 4. narrow rdmizer to exclude already found mine
    // 5. go to 2.
    
    std::vector<int> positions;

    for (int i = 0; i < dimension_x * dimension_y; i++) {
        positions.push_back(i);
    }

    int random_min = 0;
    int random_max = positions.size() - 1;

    for (int current_mine_num = 0; current_mine_num < mine_count; current_mine_num++) {
        std::uniform_int_distribution<> distr(random_min, random_max);
        int chosen_index = distr(rdm_num_machine);
        int chosen_pos = positions[chosen_index];

        // resolve chosen spot
        int x = chosen_pos % dimension_x;
        int y = chosen_pos / dimension_x;

        // put mine at spot
        mines[x][y] = true;

        // swap: chosen spot <-> first item in range
        positions[chosen_index] = positions[random_min];
        positions[random_min] = chosen_pos;

        // narrow rdmizer
        random_min++;
    }
}

void Minefield::checkPos(int x, int y) {
    if (! isPosValid(x, y)) {
        throw std::runtime_error("Given position is invalid.");
    }
}

void Minefield::checkRunning() {
    if(! isGameRunning()) {
        throw std::runtime_error("Game is not running anymore.");
    }
}

bool Minefield::isGameEnded() {
    return ! isGameRunning();
}

bool Minefield::isGameRunning() {
    // two requirements
    // 1. no mines have been opened
    // 2. there is a field to be opened that is not a mine
    
    // no mine opened
    for (int x = 0; x < getXDimension(); x++) {
        for (int y = 0; y < getYDimension(); y++) {
            if(mines[x][y] && opened[x][y]) {
                return false;
            }
        }
    }

    // an open field w/o mine exists
    for (int x = 0; x < getXDimension(); x++) {
        for (int y = 0; y < getYDimension(); y++) {
            if((! mines[x][y]) && (! opened[x][y])) {
                return true;
            }
         }
    }

    // requirements not met
    return false;
}

bool Minefield::isGameWon() {
    if (! isGameEnded()) {
        return false;
    }

    // all fields w/o mines are opened
    // -> no field opened w/o mine
    for (int x = 0; x < getXDimension(); x++) {
        for (int y = 0; y < getYDimension(); y++) {
            if ((! opened[x][y]) && (! mines[x][y])) {
                // field w/o mine has not been opened
                return false;
            }
        }
    }

    // all fields w/o mines are opened
    return true;
}

bool Minefield::isGameLost() {
    if (! isGameEnded()) {
        return false;
    }

    // there is a field w/ a mine that has been opened
    for (int x = 0; x < getXDimension(); x++) {
        for (int y = 0; y < getYDimension(); y++) {
            if (mines[x][y] && opened[x][y]) {
                return true;
            }
        }
    }
    return false;
}

bool Minefield::isPosValid(int x, int y) {
    if (x < 0 || x >= getXDimension()) {
        return false;
    }

    if (y < 0 || y >= getYDimension()) {
        return false;
    }

    return true;
}

bool Minefield::isFlagged(int x, int y) {
    checkPos(x, y);
    return flags[x][y];
}

bool Minefield::isMine(int x, int y) {
    checkPos(x, y);
    if (isGameRunning() && ! opened[x][y]) {
        throw std::runtime_error("Can't check if is mine on not-opened field.");
    }

    return mines[x][y];
}

bool Minefield::isOpen(int x, int y) {
    checkPos(x, y);
    return opened[x][y];
}

void Minefield::flag(int x, int y) {
    checkPos(x, y);
    checkRunning();

    if (opened[x][y]) {
        throw std::runtime_error("Can't place flag on opened field.");
    }
    flags[x][y] = true;
}

void Minefield::unflag(int x, int y) {
    checkPos(x, y);
    checkRunning();

    if (opened[x][y]) {
        throw std::runtime_error("Can't remove flag on opened field.");
    }

    flags[x][y] = false;
}

void Minefield::open(int x, int y, bool recursive) {
    checkPos(x, y);
    checkRunning();

    if (flags[x][y]) {
        throw std::runtime_error("Can't open given position, flag is placed.");
    }

    // check if is first spot to be opened
    if (mines[x][y] && 0 == getOpenCount()) {
        // move this mine to another open place
        std::vector<std::tuple<int, int>> emptySpots;
        for (int lx = 0; lx < getXDimension(); lx++) {
            for (int ly = 0; ly < getYDimension(); ly++) {
                if (! mines[lx][ly]) {
                    emptySpots.push_back(std::make_tuple(lx, ly));
                }
            }
        }

        // pick random empty spot
        if (0 < emptySpots.size()) {
            std::mt19937 rdm_num_machine(x * getYDimension() + y);
            std::uniform_int_distribution<> distr(0, emptySpots.size() - 1);
            int chosen_index = distr(rdm_num_machine);

            int chosen_x, chosen_y;
            std::tie(chosen_x, chosen_y) = emptySpots[chosen_index];

            // move mines
            mines[chosen_x][chosen_y] = true;
            mines[x][y] = false;
        }
    }

    opened[x][y] = true;

    if (recursive && !isMine(x, y) && (0 == getSorroundingMineCount(x, y))) {
        std::function<bool (int, int, std::vector<std::tuple<int, int>>&)> recursive_open = [&](int x, int y, std::vector<std::tuple<int, int>>& done){
            // stop on: invalid pos, already visited by recursive machine
            std::tuple<int, int> coords_tuple = {x, y};
            if (! isPosValid(x, y)) {
                return false;
            }
            if (done.end() != std::find(done.begin(), done.end(), coords_tuple)) {
               return false;
            }

            // remove flags (as only safe spots get opened via this function this is safe to do)
            flags[x][y] = false; 
            // not been visited => open (non-recursively)
            if (isGameRunning()) {
                open(x, y, false);
            }

            // only carry on if self has no sorrounding mines
            if (0 == getSorroundingMineCount(x, y)) {
                // add self to done list
                done.push_back(coords_tuple);

                // invoke self on sorrounding fields (no valid coord check, comes in next sub-call)
                for (int dx : {-1, 0, 1}) {
                    for (int dy : {-1, 0, 1}) {
                        if (isGameRunning()) {
                            recursive_open(x + dx, y + dy, done);
                        }
                    }
                }
            }

            return true;
        };

        std::vector<std::tuple<int, int>> tmp_tuple_list;
        recursive_open(x, y, tmp_tuple_list);
    }
}

int Minefield::getSorroundingMineCount(int x, int y) {
    checkPos(x, y);
    
    if (isGameRunning() && ! opened[x][y]) {
        throw std::runtime_error("Can't display sorrounding mines on unopened field while game is still running.");
    }

    int sum = 0;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx != 0 || dy != 0) {
                // ignore self
                int current_x = x + dx;
                int current_y = y + dy;

                if (isPosValid(current_x, current_y)) {
                    if (mines[current_x][current_y]) {
                        sum++;
                    }
                }
            }
        }
    }

    return sum;
}

int Minefield::getXDimension() {
    return given_x_dimension;
}

int Minefield::getYDimension() {
    return given_y_dimension;
}

int getTrueCount(std::vector<std::vector<bool>> matrix) {
    int sum = 0;
    for (unsigned long x = 0; x < matrix.size(); x++) {
        for (unsigned long y = 0; y < matrix[x].size(); y++) {
            if (matrix[x][y]) {
                sum++;
            }
        }
    }

    return sum;
}

int Minefield::getMineCount() {
    return given_mine_count;
}

int Minefield::getFlagCount() {
    return getTrueCount(flags);
}

int Minefield::getOpenCount() {
    return getTrueCount(opened);
}

int Minefield::getSeed() {
    return given_seed;
}
