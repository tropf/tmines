#ifndef __INPUTDEVICE_CURSES_H_INCLUDED__
#define __INPUTDEVICE_CURSES_H_INCLUDED__

#include <inputdevice.hpp>

class InputdeviceCurses: public Inputdevice {
    public:
        char getChar();
};

#endif // __INPUTDEVICE_CURSES_H_INCLUDED__
