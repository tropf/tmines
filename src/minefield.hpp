/// minefield class definition
/** \file
 * Contains the class definition for the minefield class.
 */
#ifndef __MINEFIELD_HPP_INCLUDED__
#define __MINEFIELD_HPP_INCLUDED__

#include <vector>

/// Implements the internal game logic
/**
 * The minefield class impplements the basic functionality of minesweeper.
 * When encountering an error, an exception will be thrown, so the minefield class should not be accessed directly, only via e.g. the controller (or display) classes.
 *
 * To keep calculation times low, a bunch of caching variables are used.
 * Therefore, the private attributes should only be directly accessed after careful code review.
 * This is unfourtunately necessary as otherwise constant iterations over the entire minefield are required.
 */
class Minefield {
    private:
        /// mine positions
        /**
         * Used to save the position of the mines, intialized in the constructor.
         * Access from outside via isMine() method.
         * Access internally directly.
         */
        std::vector<std::vector<bool>> mines;

        /// flag positions
        /**
         * Used to save the position of the flags, intialized in the constructor.
         * Access via the flag()/unflag() methods.
         */
        std::vector<std::vector<bool>> flags;

        /// opened fields
        /**
         * Saves the positions of the opened fields. Access via the open() method.
         */
        std::vector<std::vector<bool>> opened;

        /// seed used for generation
        /**
         * Stores the seed used for generation of the mine placement.
         * Kept to potentially return later.
         */
        int given_seed;

        /// caching var for mine count
        /**
         * Holds the amount of placed mines. Remains constant after initialisation through the constructor.
         */
        int given_mine_count;

        /// caching var for x dimension
        /**
         * Holds the width of the game board, so the arrays don't have to be accessed to check for dimensions.
         */
        int given_x_dimension;

        /// caching var for y dimension
        /**
         * Holds the height of the game board, so the arrays don't have to be accessed to check for dimensions.
         */
        int given_y_dimension;

        /// caching var for amount of opened fields
        /**
         * Caching var to count the opened fields.
         * Set inside of the open() method
         */
        int open_cnt;

        /// caching var for amount of placed flags
        /**
         * Holds the amount of placed flags.
         * Is updated in the flag()/unflag() methods.
         */
        int flag_cnt;

        /// caching var, true if a mine has been opened.
        /**
         * A caching var to save if a mine has been opened.
         * Greatly decreased time for checks if the game is still running.
         */
        bool opened_mine;
        
        /**
         * Throws if given position is invalid.
         * Does nothing otherwise.
         * @param x x coordinate
         * @param y y coordinate
         * @throws std::exception if the given location is invalid
         */
        void checkPos(int x, int y);

        /**
         * Throws if the game is not running.
         * @throws std::exception if the game is not running anymore
         */
        void checkRunning();
    public:
        /**
         * Creates a new Minefield.
         * Mines are automatically randomly placed.
         * Given Dimensions must both be >0
         * @param dimension_x amount of columns, index: 0..dimension_x-1
         * @param dimension_y amount of rows, index: 0..dimension_y-1
         * @param mine_count amount of mines to be placed
         * @param seed seed to initialize the random number generator
         * @throws std::exception if dimension_x or dimension_y are 0 or less, more mines should be placed than spots are available
         */
        Minefield(int dimension_x = 8, int dimension_y = 8, int mine_count = 10, int seed = 0);
        
        /**
         * Returns true if the game has ended and no more moves can be taken
         * @return true if no more turns can be taken
         */
        bool isGameEnded();
        /**
         * Returns true if the game is still running and more moves can be made.
         * @return true if more turns can be taken
         */
        bool isGameRunning();

        /**
         * Returns true if the game is over and has been won.
         * @return true if game is over & won
         */
        bool isGameWon();
        /**
         * Returns true if the game is over and has been lost.
         * (By clicking on a mine e.g.)
         * @return true if game is over & lost
         */
        bool isGameLost();

        /**
         * Returns true if the given coordinates are a valid position on the current playing field.
         * (Note that indexing starts from 0.)
         * @param x x coordinate
         * @param y y coordinate
         * @return true if the given coordinates are on the playing field.
         */
        bool isPosValid(int x, int y);
        
        /**
         * Returns true if a flag has been set at the given location.
         * Throws on invalid coordinates.
         * @param x x coordinate
         * @param y y coordinate
         * @return true if given position has a flag on it
         * @throws std::exception if the given position is invalid
         */
        bool isFlagged(int x, int y);

        /**
         * Returns true if the given position has been opened.
         * Throws on invalid coordinates.
         * @param x x coordinate
         * @param y y coordinate
         * @return true if given position has been opened
         * @throws std::exception if the given position is invalid
         */
        bool isOpen(int x, int y);

        /**
         * Returns true if the given position is a mine (and has been opened).
         * Throws on invalid coordinates.
         * Throws on not-opened spot.
         * Can be used on any spot w/o error after game end.
         * @param x x coordinate
         * @param y y coordinate
         * @return true if given position is a mince
         * @throws std::exception if the given position is invalid or has not been opened (and game is running)
         */
        bool isMine(int x, int y);

        /**
         * Places a flag on the given coordinates.
         * Throws if no Flag can be placed.
         * Can be invoked on the same coordinates twice w/o error.
         * (Can be called on coordinates with already a flag.)
         * @param x x coordinate
         * @param y y coordinate
         * @throws std::exception if no flag can be placed or the given position is invalid
         */
        void flag(int x, int y);

        /**
         * Removes a flag from the given coordinates.
         * Throws if coordinates are invalid.
         * Throws if field has been opened.
         * Can be invoked w/o error on field w/o flag.
         * @param x x coordinate
         * @param y y coordinate
         * @throws std::exception if the given position is invalid or the field has been opened
         */
        void unflag(int x, int y);

        /**
         * Opens the given field.
         * (Already Open fields can be opened again w/o error)
         * Cannot open if flag is placed.
         * If recursive is set to true (default), if the there are 0 mines on current field adjacent fields are automatically opened.
         * If recursive is set to false, only one field will be opened.
         * @param x x coordinate
         * @param y y coordinate
         * @param recursive will open sorrounding fields if current field has no sorrounding mines
         * @throws std::exception if the game is not running or given position is invalid or position is flagged
         */
        void open(int x, int y, bool recursive = true);

        /**
         * Returns the amount of the sorrounding mines (0-8).
         * Throws when the given field cannot be checked (isn't opened.)
         * Can be used on any spot w/o error after game end.
         * @param x x coordinate
         * @param y y coordinate
         * @return the number of the sorrounding mines, >=0 and <=8
         * @throws std::exception if the given position is invalid or on non-opened position before game end
         */
        int getSorroundingMineCount(int x, int y);

        /**
         * Returns the amount of columns (width of the field).
         * Note: return value == 10 -> Indexes are 0-9
         * (Index 10 doesn't exist)
         * @return the width of the playing field
         */
        int getXDimension();

        /**
         * Returns the amount of rows (height of the field).
         * Note: return value == 10 -> Indexes are 0-9
         * (Index 10 doesn't exist)
         * @return the height of the playing field
         */
        int getYDimension();

        /**
         * Returns the amount of all mines.
         * @return amount of all mines
         */
        int getMineCount();

        /**
         * Returns the amount of placed flags.
         * @return amount of placed flags
         */
        int getFlagCount();

        /**
         * Returns the amount of opened fields.
         * @return amount of opened fields
         */
        int getOpenCount();

        /**
         * Returns the seed the RNG has been initialized w/
         * @return seed given on creation
         */
        int getSeed();
};

#endif // __MINEFIELD_HPP_INCLUDED__
