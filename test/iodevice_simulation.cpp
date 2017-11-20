#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "iodevice_simulation.hpp"

#include <thread>
#include <chrono>

TEST_CASE("Add Char Test") {
    IODeviceSimulation io;
    io.addChar('a');
    io.addChar('b');
    io.addChar(17);
    io.addChar(117);
    io.addChar(42);

    CHECK('a' == io.getChar());
    CHECK('b' == io.getChar());
    CHECK(17 == io.getChar());
    CHECK(117 == io.getChar());
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
    CHECK('0' == io.getChar());

    char to_be_inserted = 'Y';
    bool thread_done = false;
    auto check_blocking_lambda = [&](){
        CHECK(io.getChar() == to_be_inserted);
        thread_done = true;
    };

    // io.getChar() should be blockig if there are no chars supplied
    std::thread check_blocking_thread(check_blocking_lambda);
    // wait so it is really blocking
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    io.addChar(to_be_inserted);
    // wait for it to finish
    check_blocking_thread.join();
    CHECK(thread_done);
}
