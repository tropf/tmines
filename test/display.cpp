#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "display.hpp"

#include "iodevice_simulation.hpp"

#include <memory>
#include <curses.h>
#include <string>

TEST_CASE("Finish on Q") {
    // prep iodevice
    IODeviceSimulation io;
    io.setDim(100, 100);
    io.addChars("q");
    io.addChars("Q");

    // run
    std::shared_ptr<IODevice> io_ptr = std::make_shared<IODeviceSimulation>(io);
    Display display(io_ptr, 8, 8, 10, 0, false);

    // check results
    auto controller = display.getController();
    auto mfield = controller.getMinefield();

    CHECK(! mfield.isOpen(3, 3));
    CHECK(!mfield.isOpen(4, 4));
    CHECK(!mfield.isOpen(0, 0));
    CHECK(3 == controller.getX());
    CHECK(3 == controller.getY());

    Display display_capital_q(io_ptr, 8, 8, 10, 0, false);
    // just finish while running
}

TEST_CASE("Movement Mappings") {
    IODeviceSimulation io;
    io.setDim(100, 100);
    // right RIGHT right down DOWN
    // start pos: 3, 3; final pos: 6, 5

    // vimlike hjkl 
    io.addChars("lLljJq");
    
    // wasd
    io.addChars("dDdsSq");

    // arrow keys
    io.addChar(KEY_RIGHT);
    io.addChar(KEY_RIGHT);
    io.addChar(KEY_RIGHT);
    io.addChar(KEY_DOWN);
    io.addChar(KEY_DOWN);
    io.addChar('q');

    // left LEFT left up UP
    // start pos: 3, 3; final pos: 0, 1
    
    // vimlike hjkl
    io.addChars("hHhkKq");

    // wasd
    io.addChars("aAawWq");

    // arrow keys
    io.addChar(KEY_LEFT);
    io.addChar(KEY_LEFT);
    io.addChar(KEY_LEFT);
    io.addChar(KEY_UP);
    io.addChar(KEY_UP);
    io.addChar('q');

    // run actual tests
    std::shared_ptr<IODevice> io_ptr = std::make_shared<IODeviceSimulation>(io);
    Display display_vim_right_down(io_ptr, 8, 8, 10, 0, false);
    Display display_wasd_right_down(io_ptr, 8, 8, 10, 0, false);
    Display display_arrow_right_down(io_ptr, 8, 8, 10, 0, false);
    Display display_vim_left_up(io_ptr, 8, 8, 10, 0, false);
    Display display_wasd_left_up(io_ptr, 8, 8, 10, 0, false);
    Display display_arrow_left_up(io_ptr, 8, 8, 10, 0, false);

    Controller con;

    con = display_vim_right_down.getController();
    CHECK(6 == con.getX());
    CHECK(5 == con.getY());

    con = display_wasd_right_down.getController();
    CHECK(6 == con.getX());
    CHECK(5 == con.getY());

    con = display_arrow_right_down.getController();
    CHECK(6 == con.getX());
    CHECK(5 == con.getY());

    con = display_vim_left_up.getController();
    CHECK(0 == con.getX());
    CHECK(1 == con.getY());

    con = display_wasd_left_up.getController();
    CHECK(0 == con.getX());
    CHECK(1 == con.getY());

    con = display_arrow_left_up.getController();
    CHECK(0 == con.getX());
    CHECK(1 == con.getY());
}

TEST_CASE("Other Mappings (Flag, redraw, open, window resize)") {
    IODeviceSimulation io;
    io.setDim(100, 100);

    // flag 4,3 4,4
    io.addChars("dFsF");
    // open 3,3; flag 3,3; -> don't actually flag because already opened
    io.addChars("aw F");
    // flag 2,2
    io.addChars("awf");
    io.addChar('q');
    std::shared_ptr<IODevice> io_ptr = std::make_shared<IODeviceSimulation>(io);
    Display display(io_ptr, 8, 8, 10, 0, false);

    auto con = display.getController();
    auto mfield = con.getMinefield();

    CHECK(mfield.isFlagged(4, 3));
    CHECK(mfield.isFlagged(4, 4));
    CHECK(mfield.isOpen(3, 3));
    CHECK(! mfield.isFlagged(3, 3));
    CHECK(mfield.isFlagged(2, 2));
    CHECK(! mfield.isFlagged(0, 0));
    CHECK(! mfield.isFlagged(1, 1));

    // get refreshcnt, initcnt etc. for normal run w/o redraws
    // -> get new io device
    IODeviceSimulation io_clean;
    io_clean.setDim(100, 100);
    io_clean.addChar('q');
    auto io_clean_ptr = std::make_shared<IODeviceSimulation>(io_clean);
    Display display_clean(io_clean_ptr, 8, 8, 10, 0, false);

    auto refresh_cnt_clean = io_clean_ptr->getRefreshCount();
    auto init_cnt_clean = io_clean_ptr->getInitCount();
    auto endwin_cnt_clean = io_clean_ptr->getEndWindowCount();
    auto clear_cnt_clean = io_clean_ptr->getClearCount();

    CHECK(init_cnt_clean == endwin_cnt_clean);

    IODeviceSimulation io_redraw;
    io_redraw.setDim(100, 100);
    io_redraw.addChars("rq");
    auto io_redraw_ptr = std::make_shared<IODeviceSimulation>(io_redraw);
    Display display_redraw(io_redraw_ptr, 8, 8, 10, 0, false);

    auto refresh_cnt_redraw = io_redraw_ptr->getRefreshCount();
    auto init_cnt_redraw = io_redraw_ptr->getInitCount();
    auto endwin_cnt_redraw = io_redraw_ptr->getEndWindowCount();
    auto clear_cnt_redraw = io_redraw_ptr->getClearCount();

    CHECK(refresh_cnt_redraw > refresh_cnt_clean);
    CHECK(init_cnt_redraw > init_cnt_clean);
    CHECK(endwin_cnt_redraw > endwin_cnt_clean);
    CHECK(clear_cnt_redraw > clear_cnt_clean);

    CHECK(init_cnt_redraw == endwin_cnt_redraw);

    // check for case insensitivity
    IODeviceSimulation io_redraw2;
    io_redraw2.setDim(100, 100);
    io_redraw2.addChars("Rq");
    auto io_redraw2_ptr = std::make_shared<IODeviceSimulation>(io_redraw2);
    Display display_redraw2(io_redraw2_ptr, 8, 8, 10, 0, false);

    auto refresh_cnt_redraw2 = io_redraw2_ptr->getRefreshCount();
    auto init_cnt_redraw2 = io_redraw2_ptr->getInitCount();
    auto endwin_cnt_redraw2 = io_redraw2_ptr->getEndWindowCount();
    auto clear_cnt_redraw2 = io_redraw2_ptr->getClearCount();

    CHECK(refresh_cnt_redraw2 == refresh_cnt_redraw);
    CHECK(init_cnt_redraw2 == init_cnt_redraw);
    CHECK(endwin_cnt_redraw2 == endwin_cnt_redraw);
    CHECK(clear_cnt_redraw2 == clear_cnt_redraw);

    CHECK(init_cnt_redraw2 == endwin_cnt_redraw2);

    // test that redraw event has no effect
    // repeat test from above

    // flag 4,3 4,4
    io.addChar(KEY_RESIZE);
    io.addChars("dFsF");
    io.addChar(KEY_RESIZE);
    // open 3,3; flag 3,3; -> don't actually flag because already opened
    io.addChars("aw F");
    io.addChar(KEY_RESIZE);
    // flag 2,2
    io.addChars("awf");
    io.addChar(KEY_RESIZE);
    io.addChar('q');
    io_ptr = std::make_shared<IODeviceSimulation>(io);
    Display display_with_resize(io_ptr, 8, 8, 10, 0, false);

    con = display.getController();
    mfield = con.getMinefield();

    CHECK(mfield.isFlagged(4, 3));
    CHECK(mfield.isFlagged(4, 4));
    CHECK(mfield.isOpen(3, 3));
    CHECK(! mfield.isFlagged(3, 3));
    CHECK(mfield.isFlagged(2, 2));
    CHECK(! mfield.isFlagged(0, 0));
    CHECK(! mfield.isFlagged(1, 1));
}

TEST_CASE("API calls") {
    // checks that the correct calls to the IODevice have been made
    auto io = std::make_shared<IODeviceSimulation>(IODeviceSimulation());
    io->setDim(100, 100);
    io->addChar('q');

    // expected: init, color mode, special keys start, color pairs, echo mode, cursor visibility
    Display display(io, 8, 8, 10, 0, false);
    
    CHECK(0 < io->getInitCount());
    CHECK(0 < io->getEndWindowCount());
    CHECK(io->getInitCount() == io->getEndWindowCount());

    CHECK(io->isColorStarted());
    CHECK(io->isSpecialKeysEnabled());
    CHECK(10 < io->getColorPairs().size());
    // set to 1 becuase game didnt end yet
    CHECK(1 == io->getCursorVisibility());
    // pressed keys should not print to the console
    CHECK(! io->isEchoMode());

    // check cursor on ended game -> hit a mine
    io->addChars("hhhkkk l l l l l l l lj h h h h h h h hj l l l l l l l l");
    io->addChar('q');
    Display(io, 8, 8, 10, 0, false);

    // game ended -> cursor invisible
    CHECK(0 == io->getCursorVisibility());
}

TEST_CASE("Dimension calculations") {
    // checks that from a given screen size the correct minefield dimensions are calculated
    auto io = std::make_shared<IODeviceSimulation>(IODeviceSimulation());
    
    // calculate console positions
    int x1, y1;
    std::tie(x1, y1) = Display::getConsolePosition(0, 0);
    int x2, y2;
    std::tie(x2, y2) = Display::getConsolePosition(0, 1);
    int x3, y3;
    std::tie(x3, y3) = Display::getConsolePosition(1, 0);
    int x4, y4;
    std::tie(x4, y4) = Display::getConsolePosition(1, 1);
    int x5, y5;
    std::tie(x5, y5) = Display::getConsolePosition(14, 80);

    // check that same x before stays same x after, independent from y
    CHECK(x1 == x2);
    CHECK(y1 != y2);

    CHECK(x1 != x3);
    CHECK(y1 == y3);

    CHECK(x1 != x4);
    CHECK(y1 != y4);
    CHECK(x3 == x4);
    CHECK(y2 == y4);

    // check for linear scaling
    auto consolex_per_unit = x3 - x1;
    auto predicted_x_14 = 14 * consolex_per_unit + x1;

    auto consoley_per_unit = y2 - y1;
    auto predicted_y_80 = 80 * consoley_per_unit + y1;

    CHECK(predicted_x_14 == x5);
    CHECK(predicted_y_80 == y5);

    // calculate window size
    
    // too small for anything (no text displayable)
    CHECK_THROWS(Display::getMaximumFieldsize(1, 1));
    CHECK_THROWS(Display::getMaximumFieldsize(0, 0));
    CHECK_THROWS(Display::getMaximumFieldsize(-1, -1));

    int width1, height1;
    std::tie(width1, height1) = Display::getRequiredWindowSize(1, 1, 1);
    io->addChar('q');
    io->setDim(width1, height1);
    CHECK_NOTHROW(Display(io, 1, 1, 1));

    int width2, height2;
    std::tie(width2, height2) = Display::getRequiredWindowSize(8, 8, 10);
    io->setDim(width2, height2);
    io->addChar('q');
    CHECK_NOTHROW(Display(io, 8, 8, 10));

    CHECK_NOTHROW(Display::getRequiredWindowSize(3, 3, 0));
    CHECK_NOTHROW(Display::getRequiredWindowSize(3, 3, 9));

    // space to print amount of mines is part of window calculations
    int width3, height3;
    // field only 1 width -> text is limiting factor
    std::tie(width3, height3) = Display::getRequiredWindowSize(1, 10, 9);

    io->addChars("qqqqqqqqqqq");
    // check crashes
    io->setDim(width1, height1);
    CHECK_NOTHROW(Display(io, 1, 1, 1));
    CHECK_THROWS(Display(io, 1, 1, 2)); // too many mines

    io->setDim(width1 - 1, height1);
    CHECK_THROWS(Display(io, 1, 1, 1));
    io->setDim(width1, height1 - 1);
    CHECK_THROWS(Display(io, 1, 1, 1));
    io->setDim(width1 - 1, height1 - 1);
    CHECK_THROWS(Display(io, 1, 1, 1));

    io->setDim(width2, height2);
    CHECK_NOTHROW(Display(io, 8, 8, 10));
    CHECK_NOTHROW(Display(io, 8, 8, 11)); // no throw, printing '10' takes same space as printing '11'

    io->setDim(width2 - 1, height2);
    CHECK_THROWS(Display(io, 8, 8, 10));
    io->setDim(width2, height2 - 1);
    CHECK_THROWS(Display(io, 8, 8, 10));
    io->setDim(width2 - 1, height2 - 1);
    CHECK_THROWS(Display(io, 8, 8, 10));

    io->setDim(width3, height3);
    CHECK_NOTHROW(Display(io, 1, 10, 8));
    CHECK_NOTHROW(Display(io, 1, 10, 9));
    // print '10 mines' instead of '9 mines' -> requires more space -> crashes
    CHECK_THROWS(Display(io, 1, 10, 10));

    io->setDim(width3 - 1, height3);
    CHECK_THROWS(Display(io, 1, 10, 9));
    io->setDim(width3, height3 - 1);
    CHECK_THROWS(Display(io, 1, 10, 9));
    io->setDim(width3 - 1, height3 - 1);
    CHECK_THROWS(Display(io, 1, 10, 9));

    // check that calculations match w/ crashes
    CHECK(Display::isWindowSizeSufficient(1, 1, 1, width1, height1));
    CHECK(! Display::isWindowSizeSufficient(1, 1, 1, width1 - 1, height1));
    CHECK(! Display::isWindowSizeSufficient(1, 1, 1, width1, height1 - 1));
    CHECK(! Display::isWindowSizeSufficient(1, 1, 1, width1 - 1, height1 - 1));

    CHECK(Display::isWindowSizeSufficient(8, 8, 10, width2, height2));
    // don't throw, printing '10 mines' takes as much space as printing '11 mines'
    CHECK(Display::isWindowSizeSufficient(8, 8, 11, width2, height2));
    CHECK(Display::isWindowSizeSufficient(8, 8, 63, width2, height2));
    CHECK(Display::isWindowSizeSufficient(8, 8, 64, width2, height2));
    CHECK(! Display::isWindowSizeSufficient(8, 8, 10, width2 - 1, height2));
    CHECK(! Display::isWindowSizeSufficient(8, 8, 10, width2, height2 - 1));
    CHECK(! Display::isWindowSizeSufficient(8, 8, 10, width2 - 1, height2 - 1));

    CHECK(Display::isWindowSizeSufficient(1, 10, 8, width3, height3));
    CHECK(Display::isWindowSizeSufficient(1, 10, 9, width3, height3));
    // print '10 mines' instead of '9 mines' -> requires more space -> doesn't work
    CHECK(! Display::isWindowSizeSufficient(1, 10, 10, width3, height3));
    CHECK(! Display::isWindowSizeSufficient(1, 10, 9, width3 - 1, height3));
    CHECK(! Display::isWindowSizeSufficient(1, 10, 9, width3 - 1, height3 - 1));
    CHECK(! Display::isWindowSizeSufficient(1, 10, 9, width3, height3 - 1));

    // check the text calculations
    int text_width_1, text_width_0, text_width_10, text_width_99, text_width_100, text_width_11, text_width_50, text_width_17, text_width_42, text_width_111;
    text_width_0    = Display::getMaxTextWidth(0);
    text_width_1    = Display::getMaxTextWidth(1);
    text_width_10   = Display::getMaxTextWidth(10);
    text_width_11   = Display::getMaxTextWidth(11);
    text_width_17   = Display::getMaxTextWidth(17);
    text_width_42   = Display::getMaxTextWidth(42);
    text_width_50   = Display::getMaxTextWidth(50);
    text_width_99   = Display::getMaxTextWidth(99);
    text_width_100  = Display::getMaxTextWidth(100);
    text_width_111  = Display::getMaxTextWidth(111);

    CHECK_THROWS(Display::getMaxTextWidth(-1));
    CHECK_THROWS(Display::getMaxTextWidth(-1337));
    CHECK_THROWS(Display::getMaxTextWidth(-17));

    CHECK(text_width_0 == text_width_1);
    CHECK(text_width_0 < text_width_10);
    CHECK(text_width_0 + 1 == text_width_10);
    CHECK(text_width_10 == text_width_11);
    CHECK(text_width_10 == text_width_17);
    CHECK(text_width_10 == text_width_42);
    CHECK(text_width_10 == text_width_50);
    CHECK(text_width_10 == text_width_99);
    CHECK(text_width_99 < text_width_100);
    CHECK(text_width_99 + 1 == text_width_100);
    CHECK(text_width_100 == text_width_111);

    // check the get maximum field size
    int min_width = std::get<0>(Display::getRequiredWindowSize(1, 1, 1));

    CHECK_THROWS(Display::getMaximumFieldsize(min_width - 1, 100));
    CHECK_NOTHROW(Display::getMaximumFieldsize(min_width, 100));
    int w1, h1, cnt1;
    std::tie(w1, h1, cnt1) = Display::getMaximumFieldsize(min_width, 100, 1);
    int w2, h2, cnt2;
    std::tie(w2, h2, cnt2) = Display::getMaximumFieldsize(min_width + 2, 100, 100);
    CHECK(w2 > w1);
    CHECK(h1 == h2);
    CHECK(cnt2 > 100);
    CHECK(cnt1 < 100);

    // recommended mine count never exceeds possible mine count
    int w3, h3, cnt3;
    std::tie(w3, h3, cnt3) = Display::getMaximumFieldsize(100, 100, 1);
    CHECK(cnt3 <= 100 * 100);

    for (int width = min_width; width < 2 * min_width; width++) {
        for (int height = 5; height < 30; height ++) {
            io->setDim(width, height);
            int field_width, field_height, mine_cnt;
            std::tie(field_width, field_height, mine_cnt) = Display::getMaximumFieldsize(width, height);
            io->addChar('q');
            CHECK_NOTHROW(Display(io, field_width, field_height, mine_cnt));
        }
    }
}

TEST_CASE("Printout") {
    // checks that the actual state of the minefield gets printed
}

TEST_CASE("Error Reports") {
    // checks that useful error reports are thrown
    auto io = std::make_shared<IODeviceSimulation>(IODeviceSimulation());
    io->setDim(100, 100);

    // check that keys are anonymized in crash reports
    io->addChars("wasdWASDhjklHJKLrRrRfFfFfF   fF");
    io->addChar(KEY_UP);
    io->addChar(KEY_LEFT);
    io->addChar(KEY_DOWN);
    io->addChar(KEY_RIGHT);

    io->addChars("FFF");

    io->addChar(KEY_RESIZE);

    // fallback
    io->addChar('q');

    // when KEY_RESIZE is pulled from stack, resize screen to 1
    io->mockResize();
    
    // containing start and end markers ">abc<" instead of "abc"
    std::string expected_msg = ">khjlkhjlhjklhjklrrrrffffff   ffkhjlfff-<";
    try {
        Display(io, 8, 8, 1);
        FAIL("Display didn't throw");
    } catch (std::exception &e) {
        // check that message contains all keypresses
        std::string err_msg(e.what());

        CHECK(err_msg.find(expected_msg) != std::string::npos);
    }
}

TEST_CASE("Check passing") {
    // check that params are actually passed to the controller and minefield
    auto io = std::make_shared<IODeviceSimulation>(IODeviceSimulation());
    io->addChars("qqqqqqqqqqqqqqqqqqqqqqq");
    io->setDim(100, 100);

    Display d1(io, 8, 8, 1, 0, false);
    auto con = d1.getController();
    CHECK(8 == con.getWidth());
    CHECK(8 == con.getHeight());
    CHECK(! con.isAutodiscoverOnly());
    auto mfield = con.getMinefield();
    CHECK(8 == mfield.getXDimension());
    CHECK(8 == mfield.getYDimension());
    CHECK(1 == mfield.getMineCount());
    CHECK(0 == mfield.getSeed());

    Display d2(io, 7, 13, 42, 1337, true);
    con = d2.getController();
    CHECK(7 == con.getWidth());
    CHECK(13 == con.getHeight());
    CHECK(con.isAutodiscoverOnly());
    mfield = con.getMinefield();
    CHECK(7 == mfield.getXDimension());
    CHECK(13 == mfield.getYDimension());
    CHECK(42 == mfield.getMineCount());
    CHECK(1337 == mfield.getSeed());

    // also errors of Mfield get thrown
    CHECK_THROWS(Display(io, 1, 1, 2));
    CHECK_NOTHROW(Display(io, 1, 1, 0));
    CHECK_THROWS(Display(io, 0, 1, 0));
    CHECK_THROWS(Display(io, 1, -1, 0));
    CHECK_THROWS(Display(io, 1, 1, -1));
}
