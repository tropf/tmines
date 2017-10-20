#ifndef __DIPLAY_H_INCLUDED__
#define __DIPLAY_H_INCLUDED__

#include "controller.hpp"

#include <tuple>

class Display {
    private:
        Controller controller;
        bool exit;

        /**
         * Renders the Board of the Game
         */
        void renderBoard();

        /**
         * Renders a status bar below the Game board
         */
        void renderStatusline();

        /**
         * Gets a input Key
         * @return key code from curses
         */
        int getKey();

        /**
         * Handles a given key, translating it into moving left/right, clicking etc.
         * @param key the pressed key from the getKey() function
         */
        void handleKey(int key);

        /**
         * starts the execution of the game
         */
        void run();

        /**
         * puts the cursor from the controller object to the console
         */
        void updateCursor();

        /**
         * Converts a given position on the game board to a position to print on the console.
         * @param x x coordinate
         * @param y y coordinate
         * @return tuple containg x and y coordinate on the console
         */
        std::tuple<int, int> getConsolePosition(int x, int y);

    public:
        /**
         * Constructor. Automatically takes over the window. (Is blocking)
         * @param width the width of the mine matrix
         * @param height the height of the mine matrix
         * @param mine_count the mine count of the mine matrix
         * @param seed seed for the RNG
         */
        Display(int width, int height, int mine_count, int seed = 0);
};

#endif //__DIPLAY_H_INCLUDED__
