#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "display.hpp"

#include "iodevice_simulation.hpp"

#include <memory>
#include <curses.h>

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
}
