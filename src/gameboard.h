#ifndef BOARD
#define BOARD

#include <SFML/Graphics.hpp>

class gameboard {
    // Data
    private:
        int numsquares;
        int nummines;

    // Methods
    public:
        gameboard(int s, int m):
            numsquares(s), 
            nummines(m)
    { }
};

#endif
