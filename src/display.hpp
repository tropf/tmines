#ifndef __DIPLAY_H_INCLUDED__
#define __DIPLAY_H_INCLUDED__

#include "controller.hpp"

#include <tuple>
#include <string>
#include <vector>
#include <exception>

/**
 * The Display class renders the minefield to the user.
 * Basically everthing is delegated to the Controller class.
 * The rendering itself is made possible by ncurses.
 */
class Display {
    private:
        Controller controller;
        bool exit;
        std::vector<std::vector<std::tuple<int, char>>> state, last_state;
        std::vector<char> pressed_keys;

        const struct {
            std::string won = "won";
            std::string lost = "lost";
            std::string running = "live";
            std::string remaining_mines = "%mine_count% mines remaining";
        } msgs;

        /**
         * Renders the Board of the Game according to state var.
         */
        void renderBoard();

        /**
         * Calculates how the board should be rendered.
         * Writes what to render into the state var.
         * Actually Print anything
         */
        void calculateStates();

        /**
         * Calculates and returns the width of the longest text (in the status bar)
         * @return width of the longest text
         */
        int getMaxTextWidth();

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

        /**
         * Checks if the current window size is sufficient to display mine field.
         * Throws if the check fails.
         */
        void checkWindowSize();

        /**
         * Takes a thrown exception and appends information to the message to reproduce the error.
         * @param e a thrown error where additional information should be added
         * @throws std::runtime_error The given error message with additional information.
         */
        void addCrashInfo(std::exception given_exception);

    public:
        /**
         * Constructor. Automatically takes over the window. (Is blocking)
         * @param width the width of the mine matrix
         * @param height the height of the mine matrix
         * @param mine_count the mine count of the mine matrix
         * @param seed seed for the RNG
         * @param autodiscover_only if enabled, fields cannot be opened directly
         */
        Display(int width, int height, int mine_count, int seed = 0, bool autodiscover_only = false);
};

#endif //__DIPLAY_H_INCLUDED__
