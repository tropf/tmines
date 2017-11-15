#ifndef __IODEVICE_SIMULATION_H_INCLUDED__
#define __IODEVICE_SIMULATION_H_INCLUDED__

#include "iodevice.hpp"

#include <string>
#include <sstream>

/**
 * Can be used to simulate user Input and screen Output for 
 *
 * > Note: If no 'q' is send in the end will not return any char and the Display class will be stuck in waiting forever (blocking the control flow).
 */
class IODeviceSimulation: public IODevice {
    private:
        std::stringstream input;
    public:
        IODeviceSimulation();

        // methods for input simulatoin

        // methods to check output

        // methods from interface

        char getChar();
        void setColor(int colorCode);
        void putString(int x, int y, std::string to_print);
        void moveCursor(int x, int y);
        void setCursorVisibility(int visibility);
        int getHeight();
        int getWidth();
        void clear();
        void refresh();
        void initWindow();
        void endWindow();
        void setEcho(bool enabled);
        void startColor();
        void startSpecialKeys();
        void addColor(int id, int fg, int bg);
        void setBackground(int id);
};

#endif // __IODEVICE_SIMULATION_H_INCLUDED__
