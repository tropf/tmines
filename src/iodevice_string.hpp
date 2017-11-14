#ifndef __IODEVICE_STRING_H_INCLUDED__
#define __IODEVICE_STRING_H_INCLUDED__

#include "iodevice.hpp"

#include <string>
#include <sstream>

/**
 * Can be used to simulate user input based on a stringstream.
 *
 * > Note: If no 'q' is send in the end will not return any char and the Display class will be stuck in waiting forever (blocking the control flow).
 */
class IODeviceString: public IODevice {
    private:
        std::stringstream input;
    public:
        /**
         * Set the Inputstream to use.
         */
        IODeviceString(std::string given_string);
        char getChar();
};

#endif // __IODEVICE_STRING_H_INCLUDED__
