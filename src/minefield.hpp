#ifndef __MINEFIELD_HPP_INCLUDED__
#define __MINEFIELD_HPP_INCLUDED__

#include <vector>

class Minefield {
    private:
        std::vector<std::vector<bool>> mines;
        std::vector<std::vector<bool>> flags;
        std::vector<std::vector<bool>> opened;
        
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
         * @throws std::exception if dimension_x or dimension_y are 0 or less
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
};

#endif // __MINEFIELD_HPP_INCLUDED__
