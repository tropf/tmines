#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "minefield.hpp"

#include <vector>
#include <tuple>
#include <algorithm>

TEST_CASE("Dimension test") {
    auto mfield = Minefield(10, 10);

    CHECK(10 == mfield.getXDimension());
    CHECK(10 == mfield.getYDimension());

    CHECK(mfield.isPosValid(0, 0));
    CHECK(mfield.isPosValid(0, 9));
    CHECK(mfield.isPosValid(9, 0));
    CHECK(! mfield.isPosValid(10, 0));
    CHECK(! mfield.isPosValid(-1, 0));
    CHECK(! mfield.isPosValid(3, -17));
    CHECK(! mfield.isPosValid(10, 10));
    CHECK(! mfield.isPosValid(1337, 42));

    CHECK_THROWS(Minefield(0, 0));
    CHECK_THROWS(Minefield(-1, 0));
    CHECK_THROWS(Minefield(10, -17));
    CHECK_NOTHROW(Minefield(10, 17));

    auto rectangular = Minefield(10, 20);
    CHECK(rectangular.isPosValid(9, 10));
    CHECK(rectangular.isPosValid(9, 19));
    CHECK(! rectangular.isPosValid(19, 9));
    CHECK(rectangular.isPosValid(1, 1));
}

TEST_CASE("Constructor Test") {
    // dimensions are already tested

    // test params being optional
    CHECK_NOTHROW(Minefield());
    CHECK_NOTHROW(Minefield(10));
    CHECK_NOTHROW(Minefield(10, 123));
    CHECK_NOTHROW(Minefield(10, 123, 11));
    CHECK_NOTHROW(Minefield(10, 123, 1132, 123123123));

    CHECK_THROWS(Minefield(0, 0));
    CHECK_NOTHROW(Minefield(1, 1, 1));
    CHECK_NOTHROW(Minefield(1, 1, 0));
    CHECK_THROWS(Minefield(1, 1, 2));

    CHECK_NOTHROW(Minefield(10, 10, 100));
    CHECK_THROWS(Minefield(10, 10, 101));
}

TEST_CASE("Game State") {
    // using seed (to make test cases consistent)
    auto mfield = Minefield(8, 8, 10, 0);

    // new game
    //CHECK(! mfield.isGameEnded());
    CHECK(! mfield.isGameEnded());
    CHECK(mfield.isGameRunning());
    CHECK(! mfield.isGameWon());
    CHECK(! mfield.isGameLost());

    // open a mine
    mfield.open(0, 5, false);
    // mine hit (by accident)
    CHECK(mfield.isGameEnded());
    CHECK(! mfield.isGameRunning());
    CHECK(! mfield.isGameWon());
    CHECK(mfield.isGameLost());

    // reinit w/ empty field
    mfield = Minefield(8, 8, 10, 0);

    // open all non-mines
    std::vector<std::tuple<int, int>> mines = {
        {5, 0},
        {7, 3},
        {3, 4},
        {5, 4},
        {6, 4},
        {0, 5},
        {3, 5},
        {6, 5},
        {6, 6},
        {7, 6},
    };

    for (int x = 0; x < mfield.getXDimension(); x++) {
        for (int y = 0; y < mfield.getYDimension(); y++) {
            std::tuple<int, int> as_tuple = {x, y};
            if (std::find(mines.begin(), mines.end(), as_tuple) == mines.end()) {
                // has not been found -> can be opened
                mfield.open(x, y, false);
            }
        }
    }
    
    // everything opened except mines
    CHECK(mfield.isGameEnded());
    CHECK(! mfield.isGameRunning());
    CHECK(mfield.isGameWon());
    CHECK(! mfield.isGameLost());
}


TEST_CASE("Open Test") {
    auto mfield = Minefield(8, 8, 10, 0);

    CHECK_THROWS(mfield.open(-1, 0));
    CHECK_THROWS(mfield.open(1, 100));

    CHECK(! mfield.isOpen(0, 0));
    mfield.open(0, 0, false);
    CHECK(mfield.isOpen(0, 0));

    CHECK(! mfield.isOpen(3, 3));
    mfield.open(3, 3, false);
    CHECK(mfield.isOpen(3, 3));
    mfield.open(3, 3, false);
    CHECK(mfield.isOpen(3, 3));
    mfield.open(3, 3, false);
    CHECK(mfield.isOpen(3, 3));

    // don't open if flagged
    mfield.flag(4, 4);
    CHECK(! mfield.isOpen(4, 4));
    CHECK(mfield.isFlagged(4, 4));
    CHECK_THROWS(mfield.open(4, 4, false));

    mfield.unflag(4, 4);
    CHECK(! mfield.isOpen(4, 4));
    CHECK(! mfield.isFlagged(4, 4));
    CHECK_NOTHROW(mfield.open(4, 4, false));

    // end game (click on mine)
    mfield.open(0, 5, false);
    CHECK(mfield.isOpen(0, 5));
    CHECK(mfield.isGameEnded());
    // don't do after game is done
    CHECK_THROWS(mfield.open(1, 1, false));
}

TEST_CASE("Un-/Flag Test") {
    auto mfield = Minefield(8, 8, 10, 0);

    CHECK_THROWS(mfield.flag(-1, 2));
    CHECK_THROWS(mfield.flag(3, 100));

    CHECK_THROWS(mfield.unflag(3, -17));
    CHECK_THROWS(mfield.unflag(100, 1));

    CHECK(! mfield.isFlagged(0, 0));
    mfield.flag(0, 0);
    CHECK(mfield.isFlagged(0, 0));

    // flagging multiple times
    CHECK(! mfield.isFlagged(3, 3));
    mfield.flag(3, 3);
    CHECK(mfield.isFlagged(3, 3));
    mfield.flag(3, 3);
    CHECK(mfield.isFlagged(3, 3));
    mfield.flag(3, 3);
    CHECK(mfield.isFlagged(3, 3));

    mfield.unflag(3, 3);
    CHECK(! mfield.isFlagged(3, 3));

    // unflagging multiple times
    CHECK(! mfield.isFlagged(2, 2));
    mfield.unflag(2, 2);
    CHECK(! mfield.isFlagged(2, 2));
    mfield.unflag(2, 2);
    CHECK(! mfield.isFlagged(2, 2));
    mfield.unflag(2, 2);

    // can't place/remove flag on opened field
    CHECK_NOTHROW(mfield.flag(1, 1));
    CHECK_NOTHROW(mfield.unflag(1, 1));

    mfield.open(1, 1, false);

    CHECK(mfield.isOpen(1, 1));
    CHECK_THROWS(mfield.flag(1, 1));
    CHECK_THROWS(mfield.unflag(1, 1));

    // cannot place/remove flags on a not running game
    mfield.open(0, 5, false);
    CHECK(mfield.isGameEnded());

    CHECK_THROWS(mfield.flag(5, 5));
    CHECK_THROWS(mfield.unflag(5, 5));
}

TEST_CASE("Sourrounding Mines") {
    auto mfield = Minefield(8, 8, 10, 0);

    // throw on wrong coords
    CHECK_THROWS(mfield.getSorroundingMineCount(-1, 1));
    CHECK_THROWS(mfield.getSorroundingMineCount(2, 100));

    // throw on unopened field
    CHECK_THROWS(mfield.getSorroundingMineCount(2, 2));
    mfield.open(2, 2, false);
    CHECK(0 == mfield.getSorroundingMineCount(2, 2));

    mfield.open(0, 4, false);
    CHECK(1 == mfield.getSorroundingMineCount(0, 4));

    mfield.open(7, 7, false);
    CHECK(2 == mfield.getSorroundingMineCount(7, 7));

    mfield.open(4, 4, false);
    CHECK(3 == mfield.getSorroundingMineCount(4, 4));

    mfield.open(7, 5, false);
    CHECK(4 == mfield.getSorroundingMineCount(7, 5));

    mfield.open(0, 0, false);
    CHECK(0 == mfield.getSorroundingMineCount(0, 0));

    mfield.open(0, 7, false);
    CHECK(0 == mfield.getSorroundingMineCount(0, 7));

    mfield.open(7, 0, false);
    CHECK(0 == mfield.getSorroundingMineCount(7, 0));

    // still working after game end
    CHECK_THROWS(mfield.getSorroundingMineCount(3, 3));
    CHECK_THROWS(mfield.getSorroundingMineCount(5, 5));
    CHECK_THROWS(mfield.getSorroundingMineCount(6, 6));

    CHECK(mfield.isGameRunning());
    mfield.open(0, 5, false);
    CHECK(mfield.isGameEnded());

    CHECK_NOTHROW(mfield.getSorroundingMineCount(0, 5));
    CHECK(0 == mfield.getSorroundingMineCount(0, 5));

    // works on any spot after game end
    CHECK_NOTHROW(mfield.getSorroundingMineCount(3, 3));
    CHECK_NOTHROW(mfield.getSorroundingMineCount(5, 5));
    CHECK_NOTHROW(mfield.getSorroundingMineCount(6, 6));
}

TEST_CASE("Is Mine") {
    auto mfield = Minefield(8, 8, 10, 0);

    CHECK_THROWS(mfield.isMine(-1, 3));
    CHECK_THROWS(mfield.isMine(1, 300));

    // has not been opened yet
    CHECK_THROWS(mfield.isMine(0, 0));
    mfield.open(0, 0, false);
    CHECK(! mfield.isMine(0, 0));

    CHECK_THROWS(mfield.isMine(1, 1));
    mfield.open(1, 1, false);
    CHECK(! mfield.isMine(1, 1));

    // works after game has been ended
    
    CHECK_THROWS(mfield.isMine(0, 5));
    CHECK_THROWS(mfield.isMine(7, 7));
    CHECK_THROWS(mfield.isMine(6, 6));

    CHECK(mfield.isGameRunning());
    mfield.open(0, 5);
    CHECK(mfield.isGameEnded());
    CHECK(mfield.isMine(0, 5));

    // works after game end on any spot
    CHECK(! mfield.isMine(7, 7));
    CHECK(mfield.isMine(6, 6));
}

TEST_CASE("Open Recursively") {
    auto mfield = Minefield(8, 8, 10, 0);

    // non-recursive
    mfield.open(0, 0, false);
    CHECK(! mfield.isOpen(1, 0));

    // recursive (default param)
    mfield.open(0, 0);
    CHECK(mfield.isOpen(1, 0));

    CHECK(mfield.isOpen(1, 4));
    CHECK(! mfield.isOpen(1, 5));

    CHECK(mfield.isOpen(5, 2));
    CHECK(! mfield.isOpen(6, 0));

    mfield.open(0, 6);
    CHECK(! mfield.isOpen(0, 7));
    CHECK(! mfield.isOpen(1, 7));

    mfield.open(1, 7);
    CHECK(mfield.isOpen(0, 7));
    CHECK(mfield.isOpen(1, 7));

    // delete flags
    mfield = Minefield(8, 8, 10, 0);

    mfield.flag(1, 1);
    CHECK(mfield.isFlagged(1, 1));
    mfield.open(0, 0);
    CHECK(! mfield.isFlagged(1, 1));
}

TEST_CASE("Copy Constructor") {
    auto mfield = Minefield(8, 8, 10, 0);

    mfield.open(0, 0);
    mfield.flag(7, 7);
    mfield.flag(6, 6);

    auto second = Minefield(mfield);
    auto third = mfield;

    CHECK(second.isOpen(0, 0));

    // compare programmatically
    for(int x = 0; x < 8; x++) {
        for (int y = 0; y < 8; y++) {
            CHECK(second.isOpen(x, y) == mfield.isOpen(x, y));
            CHECK(second.isFlagged(x, y) == mfield.isFlagged(x, y));

            CHECK(third.isOpen(x, y) == mfield.isOpen(x, y));
            CHECK(third.isFlagged(x, y) == mfield.isFlagged(x, y));
        }
    }
}
