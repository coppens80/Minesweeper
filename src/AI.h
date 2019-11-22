// SRC: J.Coppens 2019

#ifndef SWEEPERAI
#define SWEEPERAI

#include <vector>
#include "minesweeper.h"
#include "tile.h"
#include <iomanip>

class MinesweeperAI {
    // Data
    public:
        int ncols;
        int nrows;
        int num_mines;
        int num_flags;
        float tilesize; 
    private:
        std::vector<int> board_vals;
        float x, y;

    // Methods
    public:
        MinesweeperAI (Minesweeper game){
            nrows = game.nrows;
            ncols = game.ncols;
            num_mines = game.num_mines;
            num_flags = game.num_flags;
            tilesize = game.s;
            board_vals = std::vector<int>(nrows*ncols,-1);
        }

        void read_board(std::vector<GameTile> grid){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    if (grid[col + row * ncols].is_clicked)
                        board_vals[col + row * ncols] = grid[col + row * ncols].val;
                    else if (grid[col + row * ncols].flagged)
                        board_vals[col + row * ncols] = -2;
                }
            }
            print_vals();

        }

        std::pair<float,float> click_tile(int idx, int idy){
            std::cout << "index: " << idy << " " << idx << std::endl;
            x = (idx + 1.5) * tilesize;
            y = (idy + 2.5) * tilesize;
            std::cout << "coords: " <<  y << " " << x << std::endl;
            return std::pair(x,y);
        }

    private:
        void compute(void){
            return;
        }

        void print_vals(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    std::cout << std::setw(2) << board_vals[col + row * ncols] << " ";
                }
                std::cout << std::endl;
            }

        }
};

#endif
