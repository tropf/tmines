#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "controller.hpp"

#include <tuple>
#include <vector>
#include <algorithm>

TEST_CASE("Constructor Test") {
    auto con = Controller(8, 8, 10, 0);
    CHECK(8 == con.getWidth());
    CHECK(8 == con.getHeight());

    auto mfield = con.getMinefield();

    CHECK(8 == mfield.getXDimension());
    CHECK(8 == mfield.getYDimension());

    con = Controller(5, 7, 10, 0);
    CHECK(5 == con.getWidth());
    CHECK(7 == con.getHeight());

    mfield = con.getMinefield();

    CHECK(5 == mfield.getXDimension());
    CHECK(7 == mfield.getYDimension());
}

TEST_CASE("Put Cursor") {
    auto con = Controller(8, 8, 10, 0);

    // normal behaviour
    con.putCursor(5, 6);

    CHECK(5 == con.getX());
    CHECK(6 == con.getY());

    con.putCursor(0, 3);

    CHECK(0 == con.getX());
    CHECK(3 == con.getY());

    // place outside of boundaries (don't react)
    con.putCursor(10, 10);

    CHECK(0 == con.getX());
    CHECK(3 == con.getY());

    con.putCursor(-1, 3);

    CHECK(0 == con.getX());
    CHECK(3 == con.getY());

    con.putCursor(6, 100);

    CHECK(0 == con.getX());
    CHECK(3 == con.getY());
}

TEST_CASE("Movement Test") {
    auto con = Controller(8, 8, 10, 0);

    con.putCursor(0, 0);

    // follow a regular path of cursor
    con.moveRight();
    CHECK(1 == con.getX());
    CHECK(0 == con.getY());
    con.moveDown();
    con.moveDown();
    CHECK(1 == con.getX());
    CHECK(2 == con.getY());
    con.moveLeft();
    CHECK(0 == con.getX());
    CHECK(2 == con.getY());
    con.moveUp();
    CHECK(0 == con.getX());
    CHECK(1 == con.getY());

    // try to move out of boundaries...
    // left
    con.moveLeft();
    CHECK(0 == con.getX());
    CHECK(1 == con.getY());

    // up
    con.putCursor(4, 0);
    CHECK(4 == con.getX());
    CHECK(0 == con.getY());
    con.moveUp();
    CHECK(4 == con.getX());
    CHECK(0 == con.getY());

    // right
    con.putCursor(7, 1);
    CHECK(7 == con.getX());
    CHECK(1 == con.getY());
    con.moveRight();
    CHECK(7 == con.getX());
    CHECK(1 == con.getY());

    // down
    con.putCursor(5, 7);
    CHECK(5 == con.getX());
    CHECK(7 == con.getY());
    con.moveDown();
    CHECK(5 == con.getX());
    CHECK(7 == con.getY());
}

TEST_CASE("Click Test") {
    auto con = Controller(8, 8, 10, 0);

    con.putCursor(0, 0);
    con.click();
    // double clicking should work
    CHECK_NOTHROW(con.click());

    // don't open despite click when flag is placed
    con.tooggleFlag(7, 7);
    con.putCursor(7, 7);
    con.click();

    auto mfield = con.getMinefield();

    CHECK(mfield.isOpen(0, 0));
    CHECK(! mfield.isOpen(7, 7));
    CHECK(! mfield.isOpen(0, 5));

    // do nothing on invalid pos
    con.click(10, 10);
    con.click(-1, 0);
    con.click(3, 100);

    // end game
    con.click(0, 5);

    mfield = con.getMinefield();

    CHECK(mfield.isGameEnded());
    CHECK(mfield.isGameLost());

    CHECK(mfield.isOpen(0, 5));
    CHECK(mfield.isOpen(0, 0));
    CHECK(! mfield.isOpen(7, 7));

    // cannot open things anymore -> but click function should not complain
    CHECK_NOTHROW(con.click());
    con.putCursor(1, 1);
    CHECK_NOTHROW(con.click());
    con.putCursor(6, 6);
    CHECK_NOTHROW(con.click());

    mfield = con.getMinefield();
    CHECK(! mfield.isOpen(6, 6));
}

TEST_CASE("Click autodiscover feature") {
    // if an open field w/ a number is clicked
    // and a corresponding number of flags is around
    // all other fields should be opened

    auto con = Controller(8, 8, 10, 0);

    // left edge
    CHECK_NOTHROW(con.click(0, 4));
    CHECK_NOTHROW(con.click(0, 4));

    // check nothing else is open
    auto mfield = con.getMinefield();

    CHECK(! mfield.isOpen(0, 3));
    CHECK(! mfield.isOpen(1, 3));
    CHECK(! mfield.isOpen(1, 4));
    CHECK(! mfield.isOpen(1, 5));
    CHECK(! mfield.isOpen(0, 5));
    CHECK(mfield.isOpen(0, 4));
    CHECK(1 == mfield.getSorroundingMineCount(0, 4));

    CHECK_NOTHROW(con.tooggleFlag(0, 5));
    mfield = con.getMinefield();

    CHECK(! mfield.isOpen(0, 3));
    CHECK(! mfield.isOpen(1, 3));
    CHECK(! mfield.isOpen(1, 4));
    CHECK(! mfield.isOpen(1, 5));
    CHECK(! mfield.isOpen(0, 5));
    CHECK(mfield.isOpen(0, 4));

    // with disabled autodiscover
    CHECK_NOTHROW(con.click(0, 5, false));
    mfield = con.getMinefield();

    CHECK(! mfield.isOpen(0, 3));
    CHECK(! mfield.isOpen(1, 3));
    CHECK(! mfield.isOpen(1, 4));
    CHECK(! mfield.isOpen(1, 5));
    CHECK(! mfield.isOpen(0, 5));
    CHECK(mfield.isOpen(0, 4));

    // enabled autodiscover
    // this field displays 1 -> 1 flag has been placed -> click again -> open all other sorrounding fields
    CHECK_NOTHROW(con.click(0, 4));
    mfield = con.getMinefield();

    CHECK(mfield.isOpen(0, 3));
    CHECK(mfield.isOpen(1, 3));
    CHECK(mfield.isOpen(1, 4));
    CHECK(mfield.isOpen(1, 5));
    CHECK(! mfield.isOpen(0, 5));
    CHECK(mfield.isFlagged(0, 5));

    // autodiscover on finished game
    con = Controller(8, 8, 10, 0);
    mfield = con.getMinefield();

    std::vector<std::tuple<int, int>> mines = {
        std::make_tuple(5, 0),
        std::make_tuple(7, 3),
        std::make_tuple(3, 4),
        std::make_tuple(5, 4),
        std::make_tuple(6, 4),
        std::make_tuple(0, 5),
        std::make_tuple(3, 5),
        std::make_tuple(6, 5),
        std::make_tuple(6, 6),
        std::make_tuple(7, 6),
    };

    for (int x = 0; x < mfield.getXDimension(); x++) {
        for (int y = 0; y < mfield.getYDimension(); y++) {
            std::tuple<int, int> as_tuple = {x, y};
            if (std::find(mines.begin(), mines.end(), as_tuple) == mines.end()) {
                // has not been found -> can be opened
                con.tooggleFlag(x, y);
            }
        }
    }

    // flag placed on every mine -> open every spot
    for (int y = 0; y < mfield.getYDimension(); y++) {
        for (int x = 0; x < mfield.getXDimension(); x++) {
            CHECK_NOTHROW(con.click(x, y));
        }
    }
}

TEST_CASE("Toggle Flag") {
    auto con = Controller(8, 8, 10, 0);

    // invalid pos (don't react)
    CHECK_NOTHROW(con.tooggleFlag(-1, 3));
    CHECK_NOTHROW(con.tooggleFlag(3, 1337));

    // normal behaviour
    // toggle on w/ cursor
    con.putCursor(3, 4);
    con.tooggleFlag();
    auto mfield = con.getMinefield();
    CHECK(mfield.isGameRunning());
    CHECK(mfield.isFlagged(3, 4));

    // toggle off w/ direct coordinates
    con.tooggleFlag(3, 4);
    mfield = con.getMinefield();
    CHECK(mfield.isGameRunning());
    CHECK(! mfield.isFlagged(3, 4));

    // don't act on open fields
    con.click(0, 0);
    mfield = con.getMinefield();

    CHECK(mfield.isOpen(0, 0));
    CHECK(mfield.isGameRunning());
    CHECK_NOTHROW(con.tooggleFlag(0, 0));

    // don't act on ended game
    con.click(0, 5);
    con.tooggleFlag(6, 6);
    mfield = con.getMinefield();

    CHECK(! mfield.isGameRunning());
    CHECK(! mfield.isOpen(6, 6));
    CHECK(! mfield.isFlagged(6, 6));
}

TEST_CASE("First Hit No Mine") {
    auto con = Controller(8, 8, 10, 0);
    CHECK_NOTHROW(con.click(0, 5));

    auto mfield = con.getMinefield();

    CHECK(! mfield.isGameLost());
    CHECK(mfield.isGameRunning());
}

TEST_CASE("Don't open on direct click") {
    // enable only opening via autodiscover
    auto con = Controller(8, 8, 10, 0, true);
    CHECK_NOTHROW(con.click(4, 0));

    // first field can be opened directly
    auto mfield = con.getMinefield();
    CHECK(mfield.isOpen(4, 0));
    CHECK(! mfield.isOpen(3, 0));
    CHECK(! mfield.isOpen(5, 0));
    CHECK(! mfield.isOpen(3, 1));
    CHECK(! mfield.isOpen(4, 1));
    CHECK(! mfield.isOpen(5, 1));

    // after that clicking doesn't do anything
    CHECK_NOTHROW(con.click(5, 0));

    mfield = con.getMinefield();
    CHECK(mfield.isOpen(4, 0));
    CHECK(! mfield.isOpen(3, 0));
    CHECK(! mfield.isOpen(5, 0));
    CHECK(! mfield.isOpen(3, 1));
    CHECK(! mfield.isOpen(4, 1));
    CHECK(! mfield.isOpen(5, 1));

    // do something after flagging
    CHECK_NOTHROW(con.tooggleFlag(5, 0));
    CHECK_NOTHROW(con.click(4, 0));

    // everthing has been opened
    mfield = con.getMinefield();
    CHECK(mfield.isOpen(4, 0));
    CHECK(mfield.isOpen(3, 0));
    CHECK(mfield.isOpen(3, 1));
    CHECK(mfield.isOpen(4, 1));
    CHECK(mfield.isOpen(5, 1));

    CHECK(! mfield.isOpen(5, 0));
    CHECK(mfield.isFlagged(5, 0));
}
