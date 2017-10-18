#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "controller.hpp"

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

TEST_CASE("Print Test") {
    MESSAGE("TODO");
}
