#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "minefield.hpp"

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
