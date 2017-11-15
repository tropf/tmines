#include "iodevice_string.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

IODeviceString::IODeviceString(std::string given_string) {
    input << given_string;
}

char IODeviceString::getChar() {
    return input.get();
}

