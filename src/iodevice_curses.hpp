#ifndef __IODEVICE_CURSES_H_INCLUDED__
#define __IODEVICE_CURSES_H_INCLUDED__

#include "iodevice.hpp"

class IODeviceCurses: public IODevice {
    public:
        char getChar();
};

#endif // __IODEVICE_CURSES_H_INCLUDED__
