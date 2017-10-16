#ifndef __MINEFIELD_HPP_INCLUDED__
#define __MINEFIELD_HPP_INCLUDED__

#include <vector>

class Minefield {
    private:
        std::vector<std::vector<bool>> mines;
        std::vector<std::vector<bool>> flags;
        
        bool isMine(int x, int y);

    public:
        Minefield(int dimension_x, int dimension_y);
        
        bool isGameEnded();
        bool isGameRunning();

        bool isGameWon();
        bool isGameLost();

        bool isPosValid(int x, int y);
        
        bool isFlagged(int x, int y);

        void flag(int x, int y);
        void open(int x, int y);

        int getSorroundingMineCount(int x, int y);

        int getXDimension();
        int getYDimension();
}

#endif // __MINEFIELD_HPP_INCLUDED__
