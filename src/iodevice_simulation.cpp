#include "iodevice_simulation.hpp"

#include <string>
#include <sstream>
#include <stdexcept>

IODeviceSimulation::IODeviceSimulation() {
}

char IODeviceSimulation::getChar() {
    return input.get();
}

