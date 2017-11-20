#ifndef __IODEVICE_SIMULATION_H_INCLUDED__
#define __IODEVICE_SIMULATION_H_INCLUDED__

#include "iodevice.hpp"

#include <string>
#include <sstream>
#include <vector>

/**
 * Can be used to simulate user Input and screen Output for 
 *
 * > Note: If no 'q' is send in the end will not return any char and the Display class will be stuck in waiting forever (blocking the control flow).
 */
class IODeviceSimulation: public IODevice {
    private:
        std::stringstream input;

        /// foreground colors
        std::vector<std::vector<int>> foreground;

        /// background colors
        std::vector<std::vector<int>> background;

        /// printed output
        std::vector<std::vector<char>> chars;

        /**
         * Calculates, wether given coords represent a valid spot that can be printed to.
         * @param x x coordinate
         * @param y y coordinate
         * @returns wheter given pos can be printed to
         */
        bool isPosValid(int x, int y);

        /**
         * Throws if given coordinates are invalid
         * @param x x coordinate
         * @param y y coordinate
         * @throws std::exception if given position is invalid
         */
        void checkPos(int x, int y);
    public:
        // methods for input simulation content

        /**
         * Adds a char to be read from std::cin, same as pressing a button
         * @param nextchar the key to be pressed
         */
        void addChar(char nextchar);

        /**
         * Adds a lot of chars (given as string) to the input feed, same as pressing a lot of buttons
         * @param nextchars the keys to be pressed
         * to the input feed, same as pressing a lot of buttons
         * @param nextchars the keys to be pressed
         */
        void addChars(std::string nextchars);

        /**
         * Inits the field with a given dimension
         * @param width amount of chars in x direction
         * @param height amount of chars in y direction
         */
        void setDim(int width, int height);

        // methods to check output

        /**
         * Returns the internal storage for the character foreground colors.
         * @returns 2-dimensional array w/ foreground color for every character
         */
        std::vector<std::vector<int>> getForeground();

        /**
         * Returns the foreground color of a given spot
         * @param x x coordinate
         * @param y y coordinate
         * @returns foreground color at given pos
         */
        int getForeground(int x, int y);

        /**
         * Returns the internal storage for the character background colors
         * @return 2-dimensional array w/ background color for every character
         */
        std::vector<std::vector<int>> getBackground();

        /**
         * Returns the background color of a given spot
         * @param x x coordinate
         * @param y y coordinate
         * @returns background color at given pos
         */
        int getBackground(int x, int y);

        /**
         * Returns the internal storage for the printed characters
         * There may be null characters.
         * @returns 2-dimensional array w/ all printed chars (may have null characters)
         */
        std::vector<std::vector<char>> getChars();

        /**
         * Returns the char at the given pos, might be null character.
         * @param x x coordinate
         * @param y y coordinate
         * @returns character printed at the given pos, might be null
         */
        int getChars(int x, int y);

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
