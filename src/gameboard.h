#ifndef BOARD
#define BOARD

#include <SFML/Graphics.hpp>
#include <string>

class gameboard {
    // Data
    private:
        int num_squares;
        int ncols;
        int nrows;
        int num_mines;
        std::string gamemode;
        float box_size = 25.f;

    // Methods
    public:
        gameboard(std::string mode) : gamemode(mode)
    {
        std::cout << "Game mode: " << gamemode << std::endl;

        if (gamemode == "Easy"){
            ncols = 9;
            nrows = 9;
            num_mines = 10;
        }else if (gamemode == "Medium"){
            ncols = 16;
            nrows = 16;
            num_mines = 40;
        }else if (gamemode == "Hard"){
            ncols = 30;
            nrows = 16;
            num_mines = 99;
        }
        /* else if (gamemode == "Custom"){ */
        /*     ncols = 9; */
        /*     nrows = 9; */
        /*     num_mines = 10; {*/
        else
            std::cout << "Invalid game mode" << std::endl;
    }

        /* void setBoardSize(){ */
            
        /* } */

    private:

};

#endif
