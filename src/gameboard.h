#ifndef BOARD
#define BOARD

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>
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
        std::mt19937 generator;
        std::random_device rd;

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
        initBoard();
        setMines();
        setNeighbourValues();
    }

        void drawBoard(void){
            window->clear();
            for(auto& square : grid)
                window->draw(square);
            window->display();
        }

        void leftClick(const sf::Event& event) {
            for(auto& square : grid)
                square.click(event);
        }

    private:
        void initBoard(void){
            for(int row=0; row<nrows; row++){
                for(int col=0; col<ncols; col++){
                    grid.push_back(cell);
                    grid[col + row * ncols].setPosition((col+1)*cell.box_size, (row+1)*cell.box_size);
                }
            }
        }

        void setMines(void){
            generator = std::mt19937(rd());
            std::uniform_int_distribution<int> mine_idx(0, num_squares);
            for (int i=0; i<num_mines; i++)
                grid[mine_idx(generator)].createMine();
        }

        void setNeighbourValues(void){
            for(int row=0; row<nrows; row++){
                for(int col=0; col<ncols; col++){
                    int nearby_mines = 0;
                    if(grid[col + row * ncols].is_mine)
                        continue;
                    
                    for (int i=0; i<9; i++){
                        int x = col + i/3 - 1;
                        int y = row + i%3 - 1;
                    
                        if(x < 0 || x >= ncols || y < 0 || y >= nrows)
                            continue;
                        
                        if(grid[x + y * ncols].is_mine)
                            ++nearby_mines;
                    }
                    grid[col + row * ncols].setValue(nearby_mines);
                }
            }
        }
};

#endif
