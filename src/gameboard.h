#ifndef BOARD
#define BOARD

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "square.h"

class gameboard {
    // Data
    public:
        sf::RenderWindow *window;
    private:
        int num_squares;
        int ncols;
        int nrows;
        int num_mines;
        std::string gamemode;
        GridSquare cell;
        std::vector<GridSquare> grid;

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
        else
            std::cout << "Invalid game mode" << std::endl;
        num_squares = ncols * nrows;
        window = new sf::RenderWindow(sf::VideoMode(cell.box_size*(ncols+2), cell.box_size*(nrows+2)), "Minesweepy");
        grid.reserve(num_squares);
        createSquare();
    }

        void drawBoard(void){
            window->clear();
            for(auto& square : grid)
                window->draw(square);
            window->display();
        }

        void createSquare(void){
            for(int row=0; row<nrows; row++){
                for(int col=0; col<ncols; col++){
                    grid.push_back(cell);
                    grid[col + row * ncols].setPosition((col+1)*cell.box_size, (row+1)*cell.box_size);
                    grid[col + row * ncols].setValue(col);
                }
            }
        }

};

#endif
