#ifndef __CONTROLLER_H_INCLUDED__
#define __CONTROLLER_H_INCLUDED__

#include "minefield.hpp"

/**
 * The controller class servers as a driver for the minefield class.
 * The main functionality is delegated to the minefield class, the controller mainly implements the cursor object.
 * Another important feature is that the controller class *never* throws, opposed to the minefield class (which is relatively trigger happy with Exceptions).
 */
class Controller {
    private:
        Minefield mfield;
        int x, y;
        bool autodiscover_only;
    public:
        /**
         * Initializes a new Controller.
         * Passes width, height and mine count on to the minefield object.
         * @param width the width of the mine matrix
         * @param height the height of the mine matrix
         * @param mine_count the mine count of the mine matrix
         * @param seed seed for the RNG
         * @param only_autodiscover if set: will not allow to open field directly, only via autodiscover (see below)
         */
        Controller(int width = 8, int height = 8, int mine_count = 10, int seed = 0, bool only_autodiscover = false);

        /**
         * Returns the current X position of the cursor.
         * @return current x position, >=0, < width
         */
        int getX();

        /**
         * Returns the current Y position of the cursor.
         * @return current y position, >=0, < height
         */
        int getY();

        /**
         * Returns width of minefield.
         * Note that indexing starts at zero, so the field w/ index of the return value doesn't exist.
         * @return width of the minefield.
         */
        int getWidth();

        /**
         * Returns height of minefield.
         * Note that indexing starts at zero, so the field w/ index of the return value doesn't exist.
         * @return height of the minefield.
         */
        int getHeight();

        /**
         * Moves the cursor to the right.
         * Never throws, even if movement is impossible.
         */
        void moveRight();

        /**
         * Moves the cursor to the left.
         * Never throws, even if movement is impossible.
         */
        void moveLeft();

        /**
         * Moves the cursor up.
         * Never throws, even if movement is impossible.
         */
        void moveUp();

        /**
         * Moves the cursor down.
         * Never throws, even if movement is impossible.
         */
        void moveDown();

        /**
         * Puts the cursor at the specified coordinates.
         * Does nothing if invalid position.
         * @param given_x x coordinate
         * @param given_y y coordinate
         */
        void putCursor(int given_x, int given_y);

        /**
         * Opens the current position.
         * Doesn't do anything if there is a flag on the field.
         * Doesn't do anything on already open fields.
         * Doesn't do anything after the game is over.
         * Autodiscvoer is always enabled (see below).
         */
        void click();

        /**
         * Opens the given position.
         * Doesn't do anything if there is a flag on the field.
         * Doesn't do anything on already open fields.
         * Doesn't do anything after the game is over.
         * Uses autodiscover: if all sorrounding mines are flagged, all other sorrounding spots are opened.
         * @param given_x x coordinate
         * @param given_y y coordinate
         * @param autodiscover used to disable autodiscover
         */
        void click(int given_x, int given_y, bool autodiscover = true);

        /**
         * Places or removes a flag from the current position.
         * Doesn't do anything on already open fields.
         * Doesn't do anything after the game is over.
         */
        void tooggleFlag();

        /**
         * Places or removes a flag from the given position.
         * Doesn't do anything on already open fields.
         * Doesn't do anything after the game is over.
         * @param given_x x coordinate
         * @param given_y y coordinate
         */
        void tooggleFlag(int given_x, int given_y);

        /**
         * Returns the current mine field.
         * @return the used minefield
         */
        Minefield getMinefield();
};

#endif //__CONTROLLER_H_INCLUDED__
