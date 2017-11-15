#ifndef __IODEVICE_H_INCLUDED__
#define __IODEVICE_H_INCLUDED__

#include <string> 

class IODevice {
    public:
        virtual ~IODevice() {} 

        /**
         * Returns a pressed key to be handled by the display class.  
         * Is blocking.
         * @returns the pressed key
         */
        virtual char getChar() = 0;

        /**
         * Sets the print color to a predefined color pair
         * @param colorCode a given color code
         */
        virtual void setColor(int colorCode) = 0;
    
        /**
         * Prints a char to a given position.
         * @param x x corrdinate
         * @param y y coordinate
         * @param to_print the string to be printed
         */
        virtual void putString(int x, int y, std::string to_print) = 0;

        /**
         * Prints a char to a given position.
         * @param x x corrdinate
         * @param y y coordinate
         * @param to_print the string to be printed
         */
        virtual void putString(int x, int y, char to_print) {
            putString(x, y, std::string(1, to_print));
        }

        /**
         * Puts the cursor to the specified position
         * @param x x corrdinate
         * @param y y coordinate
         */
        virtual void moveCursor(int x, int y) = 0;

        /**
         * Sets the visibility of the cursor.
         * 0...hidden, 1...visible, 2...extra visible
         * @param visibility the visibility value to be set
         */
        virtual void setCursorVisibility(int visibility) = 0;

        /**
         * Returns the current height of the window.
         * @returns current height
         */
        virtual int getHeight() = 0;

        /**
         * Returns the current width of the window.
         * @returns current width
         */
        virtual int getWidth() = 0;

        /**
         * Clears the current screen
         */
        virtual void clear() = 0;

        /**
         * actually render the changes on the phyical screen.
         */
        virtual void refresh() = 0;

        /**
         * Enables the usage of the IODevice.
         *
         * Note: is not called from constructor, has to be called specifically.
         */
        virtual void initWindow() = 0;

        /**
         * Disables the usages of the IODevice and allow for other IO to happen
         *
         * Note: Is not called automatically and must be manually invoked.
         */
        virtual void endWindow() = 0;

        /**
         * Sets if the input should be printed to console.
         * Pass false to disable printing of the input.
         * @param enabled will print all input to the terminal when set to true
         */
        virtual void setEcho(bool enabled) = 0;

        /**
         * Enables the usage of colors.
         */
        virtual void startColor() = 0;

        /**
         * Activates the usage of function keys, e.g. F5, in the current screem.
         */
        virtual void startSpecialKeys() = 0;

        /**
         * Adds a color pair to the IODevice.
         * @param id unique identifier
         * @param fg foreground color
         * @param bg background color
         */
        virtual void addColor(int id, int fg, int bg) = 0;

        /**
         * Sets the background to the color of a given color pair.
         * @param id color pair identifier
         */
        virtual void setBackground(int id) = 0;
};

#endif // __IODEVICE_H_INCLUDED__
