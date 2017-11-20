#include "iodevice_simulation.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

char IODeviceSimulation::getChar() {
    return input.get();
}

