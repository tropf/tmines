/// controller method bodies
/** \file
 * Contains the method bodies for the controller class.
 */
#include "controller.hpp"

#include <tuple>

Controller::Controller(int width, int height, int mine_count, int seed, bool only_autodiscover) {
    x = 0;
    y = 0;

    mfield = Minefield(width, height, mine_count, seed);
    autodiscover_only = only_autodiscover;
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

bool Controller::isAutodiscoverOnly() {
    return autodiscover_only;
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

void Controller::click(int given_x, int given_y, bool autodiscover) {
    class InternalClicker{
        public:
            static void clicker(int given_x, int given_y, bool autodiscover, Minefield& mfield, bool only_autodiscover, bool called_during_autodiscover = false) {
                // check pos
                if (mfield.isPosValid(given_x, given_y)) {
                    // only do stuff while game is still running
                    if (mfield.isGameRunning()) {
                        // only click on fields w/o flags
                        if(! mfield.isFlagged(given_x, given_y)) {
                            if (autodiscover && mfield.isOpen(given_x, given_y)) {
                                // already open -> autodiscover
                                // if all sorrounding mines are flagged -> open all other sorrounding fields
                                int flag_count = 0;
                                for (int dx : {-1, 0, 1}) {
                                    for (int dy: {-1, 0, 1}) {
                                        if (mfield.isPosValid(given_x + dx, given_y + dy)) {
                                            if (mfield.isFlagged(given_x + dx, given_y + dy)) {
                                                flag_count++;
                                            }
                                        }
                                    }
                                }

                                // don't treat 0 sorrounding mines, because handled by open function
                                if (0 != flag_count && mfield.getSorroundingMineCount(given_x, given_y) == flag_count) {
                                    for (int dx : {-1, 0, 1}) {
                                        for (int dy: {-1, 0, 1}) {
                                            clicker(given_x + dx, given_y + dy, false, mfield, only_autodiscover, true);
                                        }
                                    }
                                }
                            } else {
                                // no autodiscover -> open regularly
                                // unless autodiscover only enable
                                // unless called from autodiscover or no field opened yet
                                if (called_during_autodiscover || 0 == mfield.getOpenCount() || !only_autodiscover) {
                                    mfield.open(given_x, given_y);
                                }
                            }
                        }
                    }
                }
            }
    };

    // delegate to hide the called from autodiscover param from outside
    InternalClicker::clicker(given_x, given_y, autodiscover, mfield, autodiscover_only);
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

Minefield Controller::getMinefield() {
    return mfield;
}

