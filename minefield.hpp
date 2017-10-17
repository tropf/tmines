#ifndef __MINEFIELD_HPP_INCLUDED__
#define __MINEFIELD_HPP_INCLUDED__

#include <vector>

class Minefield {
    private:
        std::vector<std::vector<bool>> mines;
        std::vector<std::vector<bool>> flags;
        
        /**
         * returns true if there is a mine at the given location
         * @param x x coordinate
         * @param y y coordinate
         * @return true if at the given location is a mine, false otherwise
         * @throws std::exception if the given location is invalid
         */
        bool isMine(int x, int y);

    public:
        /**
         * Creates a new Minefield.
         * Mines are automatically randomly placed.
         * Given Dimensions must both be >0
         * @param dimension_x amount of columns, index: 0..dimension_x-1
         * @param dimension_y amount of rows, index: 0..dimension_y-1
         * @throws std::exception if dimension_x or dimension_y are 0 or less
         */
        Minefield(int dimension_x, int dimension_y);
        
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

        void flag(int x, int y);
        void open(int x, int y);

        int getSorroundingMineCount(int x, int y);

        int getXDimension();
        int getYDimension();
};

#endif // __MINEFIELD_HPP_INCLUDED__
