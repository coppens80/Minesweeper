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
        std::vector<std::vector<int>> board_vals;
        float x, y;

    // Methods
    public:
        MinesweeperAI (Minesweeper game){
            nrows = game.nrows;
            ncols = game.ncols;
            num_mines = game.num_mines;
            num_flags = game.num_flags;
            tilesize = game.s;
            board_vals = std::vector<std::vector<int>>(nrows, std::vector<int>(ncols,-1));
        }

        void read_board(std::vector<GameTile> grid){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    if (grid[col + row * ncols].is_clicked)
                        board_vals[row][col] = grid[col + row * ncols].val;
                    else if (grid[col + row * ncols].flagged)
                        board_vals[row][col] = -2;
                }
            }
            print_vals();
        }

        std::tuple<float,float,bool> click_tile(int idx, int idy, bool flag){
            //std::cout << "index: " << idy << " " << idx << std::endl;
            x = (idx + 1.5) * tilesize;
            y = (idy + 2.5) * tilesize;
            //std::cout << "coords: " <<  y << " " << x << std::endl;
            return std::tuple(x,y,flag);
        }

        /* void step(void){ */
        /*     read_board(); */
        /*     compute(); */
        /*     click_tile(); */
        /* } */

    private:
        void compute(void){
            return;
        }

        void print_vals(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    std::cout << std::setw(2) << board_vals[row][col] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
};

#endif
