#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest/doctest.h"

#include "iodevice_simulation.hpp"

#include <thread>
#include <chrono>
#include <curses.h>

TEST_CASE("Add Char Test") {
    IODeviceSimulation io;
    io.addChar('a');
    io.addChar('b');
    io.addChar(44);
    io.addChar(77);
    io.addChar(42);

    CHECK('a' == io.getChar());
    CHECK('b' == io.getChar());
    CHECK(44 == io.getChar());
    CHECK(77 == io.getChar());
    CHECK(42 == io.getChar());

    io.addChars("");
    io.addChars("Hello World!\n");
    io.addChars("");
    CHECK('H' == io.getChar());
    CHECK('e' == io.getChar());
    CHECK('l' == io.getChar());
    CHECK('l' == io.getChar());
    CHECK('o' == io.getChar());
    CHECK(' ' == io.getChar());

    io.addChar(0);
    io.addChars("");

    CHECK('W' == io.getChar());
    CHECK('o' == io.getChar());
    CHECK('r' == io.getChar());
    CHECK('l' == io.getChar());
    CHECK('d' == io.getChar());
    CHECK('!' == io.getChar());
    CHECK('\n' == io.getChar());
    CHECK(0 == io.getChar());

    // test on non-chars
    // (compatibility to curses KEY_ consts)
    io.addChar(1337);
    io.addChars("Alf");
    io.addChar(238);
    io.addChar(-1);
    io.addChar(0);
    
    CHECK(1337 == io.getChar());
    CHECK('A' == io.getChar());
    CHECK('l' == io.getChar());
    CHECK('f' == io.getChar());
    CHECK(238 == io.getChar());
    CHECK(-1 == io.getChar());
    CHECK(0 == io.getChar());

    // wait so it is really blocking
    io.addChars("Y");
    CHECK('Y' == io.getChar());
}

TEST_CASE("Dimensions Test, setDim()+isPosValid()") {
    IODeviceSimulation io;
    io.initWindow();

    io.setDim(5, 5);

    CHECK(5 == io.getWidth());
    CHECK(5 == io.getHeight());
    CHECK(io.isPosValid(4, 4));
    CHECK_NOTHROW(io.putString(0, 2, "01234"));
    CHECK_NOTHROW(io.putString(2, 2, "234"));
    CHECK_THROWS(io.putString(0, 2, "01234-"));
    CHECK_THROWS(io.putString(2, 2, "234-"));

    CHECK(! io.isPosValid(5, 4));
    CHECK(! io.isPosValid(4, 5));
    CHECK(! io.isPosValid(5, 5));

    CHECK(io.isPosValid(0, 0));
    CHECK(! io.isPosValid(-1, 0));
    CHECK(io.isPosValid(0, 1));
    CHECK(! io.isPosValid(0, -1));
    CHECK(! io.isPosValid(-1, -1));

    io.setDim(4, 8);
    CHECK(4 == io.getWidth());
    CHECK(8 == io.getHeight());
    CHECK(! io.isPosValid(4, 8));
    CHECK(io.isPosValid(3, 7));
    CHECK(io.isPosValid(3, 3));

    // fill w/ example data
    CHECK(! io.isColorStarted());
    io.startColor();
    CHECK(io.isColorStarted());
    io.setDim(10, 10);
    io.addColor(17, 0, 7);
    io.addColor(42, 4, 5);
    io.setColor(17);
    io.putString(0, 0, "0123456789");
    io.setColor(42);
    io.putString(0, 1, "abcdefgh");

    CHECK_THROWS(io.putString(0, 2, "76126187269138746193413"));
    CHECK_THROWS(io.putString(0, 2, "0123456789ß"));
    CHECK_NOTHROW(io.putString(0, 2, "0123456789"));

    CHECK_THROWS(io.putString(5, 2, "0123456789"));
    CHECK_THROWS(io.putString(1, 2, "0123456789"));
    CHECK_NOTHROW(io.putString(1, 2, "012345678"));

    io.setDim(9, 1);
    CHECK(9 == io.getWidth());
    CHECK(1 == io.getHeight());

    CHECK(! io.isPosValid(9, 1));
    CHECK(io.isPosValid(8, 0));

    // check that data is kept
    CHECK('0' == io.getPrintedChar(0, 0));
    CHECK('1' == io.getPrintedChar(1, 0));
    CHECK('2' == io.getPrintedChar(2, 0));
    CHECK('3' == io.getPrintedChar(3, 0));
    CHECK(0 == io.getForeground(0, 0));
    CHECK(0 == io.getForeground(1, 0));
    CHECK(0 == io.getForeground(2, 0));
    CHECK(7 == io.getBackground(0, 0));
    CHECK(7 == io.getBackground(1, 0));
    CHECK(7 == io.getBackground(2, 0));

    // check internal array creation
    // "nothrow" is an understatement, it's gonna segfault in case of struggle
    auto chars = io.getPrintedChars();
    CHECK_NOTHROW(chars[0][0]);
    CHECK_NOTHROW(chars[8][0]);
    auto fg = io.getForeground();
    CHECK_NOTHROW(fg[0][0]);
    CHECK_NOTHROW(fg[8][0]);
    auto bg = io.getBackground();
    CHECK_NOTHROW(bg[0][0]);
    CHECK_NOTHROW(bg[8][0]);

    CHECK_NOTHROW(io.getPrintedChar(8, 0));
    CHECK_THROWS(io.getPrintedChar(9, 0));
    
    CHECK_THROWS(io.setDim(0, 0));
    CHECK_THROWS(io.setDim(0, 6));
    CHECK_THROWS(io.setDim(6, 0));

    // indirectly test checkPos
    io.setDim(1, 1);
    CHECK_NOTHROW(io.getPrintedChar(0, 0));
    CHECK_NOTHROW(io.moveCursor(0, 0));
    
    CHECK_THROWS(io.moveCursor(1, 0));
}

TEST_CASE("Color Test") {
    IODeviceSimulation io;
    io.setDim(10, 10);
    io.initWindow();

    CHECK_THROWS(io.addColor(0, 0, 10));
    CHECK_THROWS(io.setColor(0));

    CHECK(! io.isColorStarted());
    io.startColor();
    CHECK(io.isColorStarted());

    io.addColor(0, 0, 10);
    io.addColor(1, 1, 11);
    io.addColor(2, 2, 12);
    io.addColor(17, 0, 7);
    io.addColor(42, 4, 5);

    auto colors = io.getColorPairs();
    CHECK(0 == std::get<0>(colors[0]));
    CHECK(10 == std::get<1>(colors[0]));
    CHECK(1 == std::get<0>(colors[1]));
    CHECK(11 == std::get<1>(colors[1]));
    CHECK(2 == std::get<0>(colors[2]));
    CHECK(12 == std::get<1>(colors[2]));
    CHECK(0 == std::get<0>(colors[17]));
    CHECK(7 == std::get<1>(colors[17]));
    CHECK(4 == std::get<0>(colors[42]));
    CHECK(5 == std::get<1>(colors[42]));

    io.setColor(0);
    io.putString(0, 0, "012345");
    io.setColor(1);
    io.putString(1, 1, "12345");
    io.setColor(2);
    io.putString(2, 2, "2345");
    io.setColor(17);
    io.putString(3, 3, "345");
    io.setColor(42);
    io.putString(4, 4, "45");

    CHECK_THROWS(io.setColor(-1));
    CHECK_THROWS(io.setColor(20));

    auto fg = io.getForeground();
    auto bg = io.getBackground();
    auto chars = io.getPrintedChars();

    CHECK('0'   == chars[0][0]);
    CHECK(0     == fg   [0][0]);
    CHECK(10    == bg   [0][0]);
    CHECK('1'   == chars[1][0]);
    CHECK(0     == fg   [1][0]);
    CHECK(10    == bg   [1][0]);
    CHECK('2'   == chars[2][0]);
    CHECK(0     == fg   [2][0]);
    CHECK(10    == bg   [2][0]);

    CHECK('1'   == chars[1][1]);
    CHECK(1     == fg   [1][1]);
    CHECK(11    == bg   [1][1]);
    CHECK('2'   == chars[2][1]);
    CHECK(1     == fg   [2][1]);
    CHECK(11    == bg   [2][1]);
    CHECK('3'   == chars[3][1]);
    CHECK(1     == fg   [3][1]);
    CHECK(11    == bg   [3][1]);

    CHECK('2'   == chars[2][2]);
    CHECK(2     == fg   [2][2]);
    CHECK(12    == bg   [2][2]);
    CHECK('3'   == chars[3][2]);
    CHECK(2     == fg   [3][2]);
    CHECK(12    == bg   [3][2]);
    CHECK('4'   == chars[4][2]);
    CHECK(2     == fg   [4][2]);
    CHECK(12    == bg   [4][2]);

    CHECK('3'   == chars[3][3]);
    CHECK(0     == fg   [3][3]);
    CHECK(7     == bg   [3][3]);
    CHECK('4'   == chars[4][3]);
    CHECK(0     == fg   [4][3]);
    CHECK(7     == bg   [4][3]);
    CHECK('5'   == chars[5][3]);
    CHECK(0     == fg   [5][3]);
    CHECK(7     == bg   [5][3]);

    CHECK('4'   == chars[4][4]);
    CHECK(4     == fg   [4][4]);
    CHECK(5     == bg   [4][4]);
    CHECK('5'   == chars[5][4]);
    CHECK(4     == fg   [5][4]);
    CHECK(5     == bg   [5][4]);

    // check background
    io.setBackground(0);
    CHECK(0 == io.getForeground(9, 9));
    CHECK(10 == io.getBackground(9, 9));

    CHECK(0 == io.getForeground(0, 0));
    CHECK(1 == io.getForeground(1, 1));
    CHECK(2 == io.getForeground(2, 2));
    CHECK(0 == io.getForeground(3, 3));
    CHECK(4 == io.getForeground(4, 4));

    CHECK(10 == io.getBackground(0, 0));
    CHECK(11 == io.getBackground(1, 1));
    CHECK(12 == io.getBackground(2, 2));
    CHECK(7 == io.getBackground(3, 3));
    CHECK(5 == io.getBackground(4, 4));

    io.setBackground(1);
    CHECK(1 == io.getForeground(9, 9));
    CHECK(11 == io.getBackground(9, 9));

    CHECK(0 == io.getForeground(0, 0));
    CHECK(1 == io.getForeground(1, 1));
    CHECK(2 == io.getForeground(2, 2));
    CHECK(0 == io.getForeground(3, 3));
    CHECK(4 == io.getForeground(4, 4));

    CHECK(10 == io.getBackground(0, 0));
    CHECK(11 == io.getBackground(1, 1));
    CHECK(12 == io.getBackground(2, 2));
    CHECK(7 == io.getBackground(3, 3));
    CHECK(5 == io.getBackground(4, 4));

    io.setBackground(2);
    CHECK(2 == io.getForeground(9, 9));
    CHECK(12 == io.getBackground(9, 9));

    CHECK(0 == io.getForeground(0, 0));
    CHECK(1 == io.getForeground(1, 1));
    CHECK(2 == io.getForeground(2, 2));
    CHECK(0 == io.getForeground(3, 3));
    CHECK(4 == io.getForeground(4, 4));

    CHECK(10 == io.getBackground(0, 0));
    CHECK(11 == io.getBackground(1, 1));
    CHECK(12 == io.getBackground(2, 2));
    CHECK(7 == io.getBackground(3, 3));
    CHECK(5 == io.getBackground(4, 4));

    io.setBackground(17);
    CHECK(0 == io.getForeground(9, 9));
    CHECK(7 == io.getBackground(9, 9));

    CHECK(0 == io.getForeground(0, 0));
    CHECK(1 == io.getForeground(1, 1));
    CHECK(2 == io.getForeground(2, 2));
    CHECK(0 == io.getForeground(3, 3));
    CHECK(4 == io.getForeground(4, 4));

    CHECK(10 == io.getBackground(0, 0));
    CHECK(11 == io.getBackground(1, 1));
    CHECK(12 == io.getBackground(2, 2));
    CHECK(7 == io.getBackground(3, 3));
    CHECK(5 == io.getBackground(4, 4));

    io.setBackground(42);
    CHECK(4 == io.getForeground(9, 9));
    CHECK(5 == io.getBackground(9, 9));

    CHECK(0 == io.getForeground(0, 0));
    CHECK(1 == io.getForeground(1, 1));
    CHECK(2 == io.getForeground(2, 2));
    CHECK(0 == io.getForeground(3, 3));
    CHECK(4 == io.getForeground(4, 4));

    CHECK(10 == io.getBackground(0, 0));
    CHECK(11 == io.getBackground(1, 1));
    CHECK(12 == io.getBackground(2, 2));
    CHECK(7 == io.getBackground(3, 3));
    CHECK(5 == io.getBackground(4, 4));

    CHECK_THROWS(io.setBackground(13));
    CHECK_THROWS(io.setBackground(3));
    CHECK_THROWS(io.setBackground(-1));
}

TEST_CASE("Move Cursor") {
    IODeviceSimulation io;
    io.setDim(10, 10);
    io.initWindow();

    CHECK_NOTHROW(io.moveCursor(0, 0));
    CHECK(0 == io.getCursorX());
    CHECK(0 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(9, 0));
    CHECK(9 == io.getCursorX());
    CHECK(0 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(0, 9));
    CHECK(0 == io.getCursorX());
    CHECK(9 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(9, 9));
    CHECK(9 == io.getCursorX());
    CHECK(9 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(4, 5));
    CHECK(4 == io.getCursorX());
    CHECK(5 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(2, 2));
    CHECK(2 == io.getCursorX());
    CHECK(2 == io.getCursorY());

    CHECK_THROWS(io.moveCursor(10, 0));
    CHECK_THROWS(io.moveCursor(0, 10));
    CHECK_THROWS(io.moveCursor(10, 10));
    CHECK_THROWS(io.moveCursor(-1, 1));
    CHECK_THROWS(io.moveCursor(1, -1));
    CHECK_THROWS(io.moveCursor(-1, -1));

    io.setDim(3, 8);

    CHECK_NOTHROW(io.moveCursor(0, 0));
    CHECK(0 == io.getCursorX());
    CHECK(0 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(2, 0));
    CHECK(2 == io.getCursorX());
    CHECK(0 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(0, 7));
    CHECK(0 == io.getCursorX());
    CHECK(7 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(2, 7));
    CHECK(2 == io.getCursorX());
    CHECK(7 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(1, 5));
    CHECK(1 == io.getCursorX());
    CHECK(5 == io.getCursorY());

    CHECK_NOTHROW(io.moveCursor(1, 1));
    CHECK(1 == io.getCursorX());
    CHECK(1 == io.getCursorY());

    CHECK_THROWS(io.moveCursor(3, 0));
    CHECK_THROWS(io.moveCursor(0, 8));
    CHECK_THROWS(io.moveCursor(3, 8));
    CHECK_THROWS(io.moveCursor(1, 1337));
    CHECK_THROWS(io.moveCursor(-1, 1));
    CHECK_THROWS(io.moveCursor(1, -1));
    CHECK_THROWS(io.moveCursor(-1, -1));
}

TEST_CASE("Set Visibility") {
    IODeviceSimulation io;
    io.initWindow();

    CHECK_NOTHROW(io.setCursorVisibility(0));
    CHECK(0 == io.getCursorVisibility());

    CHECK_NOTHROW(io.setCursorVisibility(1));
    CHECK(1 == io.getCursorVisibility());

    CHECK_NOTHROW(io.setCursorVisibility(2));
    CHECK(2 == io.getCursorVisibility());

    CHECK_THROWS(io.setCursorVisibility(-1));
    CHECK_THROWS(io.setCursorVisibility(-7));
    CHECK_THROWS(io.setCursorVisibility(3));
    CHECK_THROWS(io.setCursorVisibility(13));
    CHECK_THROWS(io.setCursorVisibility(17));
    CHECK_THROWS(io.setCursorVisibility(1337));
}

TEST_CASE("Test Clear") {
    IODeviceSimulation io;
    io.setDim(10, 10);
    io.initWindow();
    
    io.putString(0, 0, "Hallo");
    io.putString(1, 1, "Welt!");

    auto charsbefore = io.getPrintedChars();

    CHECK('H' == charsbefore[0][0]);
    CHECK('a' == charsbefore[1][0]);
    CHECK('W' == charsbefore[1][1]);
    CHECK('!' == charsbefore[5][1]);

    CHECK(0 == io.getClearCount());
    CHECK(0 == io.getRefreshCount());

    io.clear();
    CHECK(1 == io.getClearCount());
    CHECK(0 == io.getRefreshCount());

    io.refresh();
    CHECK(1 == io.getClearCount());
    CHECK(1 == io.getRefreshCount());

    auto charsafter = io.getPrintedChars();
    CHECK(0 == charsafter[0][0]);
    CHECK(0 == charsafter[1][0]);
    CHECK(0 == charsafter[1][1]);
    CHECK(0 == charsafter[5][1]);
}

TEST_CASE("init & end") {
    IODeviceSimulation io;

    CHECK_NOTHROW(io.setDim(10, 10));

    CHECK(0 == io.getInitCount());
    CHECK(0 == io.getEndWindowCount());

    CHECK_THROWS(io.putString(0, 0, "Hallo W!"));

    io.initWindow();

    CHECK(1 == io.getInitCount());
    CHECK(0 == io.getEndWindowCount());

    CHECK_NOTHROW(io.putString(0, 0, "Hallo W!"));

    io.endWindow();

    CHECK(1 == io.getInitCount());
    CHECK(1 == io.getEndWindowCount());

    CHECK_THROWS(io.putString(0, 0, "Hallo W!"));


    CHECK_THROWS(io.putString(0, 0, "Hallo W!"));

    io.initWindow();
    CHECK(2 == io.getInitCount());
    CHECK(1 == io.getEndWindowCount());

    CHECK_NOTHROW(io.putString(0, 0, "Hallo W!"));
    CHECK_NOTHROW(io.putString(1, 1, "Hallo W!"));

    io.endWindow();
    CHECK(2 == io.getInitCount());
    CHECK(2 == io.getEndWindowCount());

    CHECK_THROWS(io.putString(0, 0, "Hallo W!"));
    CHECK_THROWS(io.putString(1, 1, "Hallo W!"));
}

TEST_CASE("misc") {
    // test that function exists
    IODeviceSimulation io;
    io.initWindow();

    CHECK(! io.isSpecialKeysEnabled());
    CHECK_NOTHROW(io.startSpecialKeys());
    CHECK(io.isSpecialKeysEnabled());

    CHECK_NOTHROW(io.setEcho(true));
    CHECK(io.isEchoMode());
    CHECK_NOTHROW(io.setEcho(true));
    CHECK_NOTHROW(io.setEcho(false));
    CHECK(! io.isEchoMode());

    io.endWindow();
}

TEST_CASE("mock resize") {
    IODeviceSimulation io;
    io.setDim(13, 37);
    io.initWindow();
    io.addChar(KEY_RESIZE);
    io.addChar('a');
    io.addChar(KEY_RESIZE);
    io.addChar(KEY_RESIZE);
    io.addChar(KEY_RESIZE);

    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());

    CHECK(KEY_RESIZE == io.getChar());

    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());

    io.mockResize();

    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());

    CHECK('a' == io.getChar());

    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());
    CHECK(13 == io.getWidth());

    CHECK(KEY_RESIZE == io.getChar());

    CHECK(1 == io.getWidth());
    CHECK(1 == io.getHeight());
    CHECK(1 == io.getWidth());
    CHECK(1 == io.getHeight());

    io.mockResize(1);
    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());

    CHECK(KEY_RESIZE == io.getChar());

    CHECK(1 == io.getHeight());
    CHECK(1 == io.getWidth());

    io.mockResize(0);
    CHECK(1 == io.getWidth());
    CHECK(1 == io.getHeight());

    io.mockResize(-1);
    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
    
    CHECK(KEY_RESIZE == io.getChar());

    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
    CHECK(13 == io.getWidth());
    CHECK(37 == io.getHeight());
}
