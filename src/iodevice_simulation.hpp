#ifndef __IODEVICE_SIMULATION_H_INCLUDED__
#define __IODEVICE_SIMULATION_H_INCLUDED__

#include "iodevice.hpp"

#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <tuple>
#include <list>

/**
 * Can be used to simulate user Input and screen Output for 
 *
 * > Note: If no 'q' is send in the end will not return any char and the Display class will be stuck in waiting forever (blocking the control flow).
 */
class IODeviceSimulation: public IODevice {
    private:
        /// the keys pressed
        /**
         * The Keys Pressed
         * Note: Stored as ints for compatibility to curses KEY_ consts.
         */
        std::list<int> input;

        /// foreground colors
        std::vector<std::vector<int>> foreground;

        /// background colors
        std::vector<std::vector<int>> background;

        /// printed output
        std::vector<std::vector<char>> chars;

        /// wether color mode is active
        bool colorMode = false;

        /// wether curses mode is activated (any output can be done)
        bool windowActive = false;

        /// code of current color
        int currentColor = 0;

        /// color pairs id -> (foreground, background)
        std::map<int, std::tuple<int, int>> colors = {
            {0, std::make_tuple(0, 0)}
        };

        /// cursor pos x coordinat
        int cursorX;

        /// cursor pos y coordinat
        int cursorY;

        /// the set cursor visibility
        /**
         * the given cursor visibility
         *
         * Note: initialized to one, "visible" (0 is invisible)
         */
        int cursorVisibility = 1;

        /// how often the window has been cleared
        int clearCnt = 0;

        /// how often refresh() has been called
        int refreshCnt = 0;

        /// how often initWindow() has been called
        int initCnt = 0;

        /// how often endWindow() has been called
        int endWinCnt = 0;

        /// the given echo mode
        bool echoMode = true;

        /// wether special keys have been enabled
        bool specialKeysEnabled = false;

        /**
         * Throws if given coordinates are invalid
         * @param x x coordinate
         * @param y y coordinate
         * @throws std::exception if given position is invalid
         */
        void checkPos(int x, int y);

        /**
         * Throws if colormode is not currently enabled.
         * @throws std::exception if color mode is disabled
         */
        void checkColorMode();

        /**
         * Throws if the curses mode is not activated == initWindow has not been called and no i/o action can be performed.
         * @throws std::exception if initWindow has not been called
         */
        void checkWindowActive();

    public:
        // methods for input simulation content

        /**
         * Calculates, wether given coords represent a valid spot that can be printed to.
         * @param x x coordinate
         * @param y y coordinate
         * @returns wheter given pos can be printed to
         */
        bool isPosValid(int x, int y);

        /**
         * Adds a char to be read from std::cin, same as pressing a button
         *
         * Note: Given as int for compatibility to curses KEY_ consts.  
         * Note: Never printed to virtual console, even w/ echo mode enabled (contrary to default console behaviour)
         * @param nextchar the key to be pressed
         */
        void addChar(int nextchar);

        /**
         * Adds a lot of chars (given as string) to the input feed, same as pressing a lot of buttons
         * to the input feed, same as pressing a lot of buttons
         *
         * Note: Never printed to virtual console, even w/ echo mode enabled (contrary to default console behaviour)
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
        std::vector<std::vector<char>> getPrintedChars();

        /**
         * Returns the char at the given pos, might be null character.
         * @param x x coordinate
         * @param y y coordinate
         * @returns character printed at the given pos, might be null
         */
        int getPrintedChar(int x, int y);

        /**
         * Retrieves the current position of the cursor on the virtual console.
         * @returns the X coordinate on the console of the cursor
         */
        int getCursorX();

        /**
         * Retrieves the current position of the cursor on the virtual console.
         * @returns the X coordinate on the console of the cursor
         */
        int getCursorY();

        /**
         * Retrieves the cursors visibility value
         * @returns the set visibility value
         */
        int getCursorVisibility();

        /**
         * Returns how often the clear() method has been called
         * @returns the amount the clear() method has been called
         */
        int getClearCount();

        /**
         * The Number of times refresh() has been called
         * @returns the amount refresh() has been called
         */
        int getRefreshCount();

        /**
         * The Number of times init() method has been called
         * @returns the amount init() has been called
         */
        int getInitCount();

        /**
         * The number of times the endWindow() method has been called
         * @returns the amount endWindow() has been called
         */
        int getEndWindowCount();

        /**
         * If the echo mode is enabled.
         * @returns the set echo mode
         */
        bool isEchoMode();

        /**
         * Wether the color mode has been enabled
         * @returns true if startColor() has been called
         */
        bool isColorStarted();

        /**
         * Wether special keys have been enabled
         *
         * Note: Gramatically incorrect but kept for code style reasons
         * @returns if startSpecialKeys() has been called
         */
        bool isSpecialKeysEnabled();

        /**
         * Returns the internal storage of the color pairs as map.  
         * `id-> (foreground, background)`
         * @returns the added color pairs
         */
        std::map<int, std::tuple<int, int>> getColorPairs();

        // methods from interface

        int getChar();
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
