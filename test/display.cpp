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
    // open 3,3; flag 3,3
    io.addChars("aw F");
    // flag 2,2
    io.addChars("awf");
    std::shared_ptr<IODevice> io_ptr = std::make_shared<IODeviceSimulation>(io);
    Display display(io_ptr, 8, 8, 10, 0, false);

}
