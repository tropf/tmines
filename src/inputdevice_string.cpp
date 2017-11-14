#include "inputdevice_string.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

InputdeviceString::InputdeviceString(std::string given_string) {
    input << given_string;
}

char InputdeviceString::getChar() {
    return input.get();
}

