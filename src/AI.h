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
        Minesweeper *game;

    // Methods
    public:
        MinesweeperAI (Minesweeper* gameref){
            game = gameref;
            nrows = game->nrows;
            ncols = game->ncols;
            num_mines = game->num_mines;
            num_flags = game->num_flags;
            tilesize = game->s;
            board_vals = std::vector<std::vector<int>>(nrows, std::vector<int>(ncols,-1));
        }

        void read_board(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    if (game->grid[col + row * ncols].is_clicked)
                        board_vals[row][col] = game->grid[col + row * ncols].val;
                    else if (game->grid[col + row * ncols].flagged)
                        board_vals[row][col] = -2;
                    else
                        board_vals[row][col] = -1;
                }
            }
            /* print_vals(); */
        }

        void click_tile(int idy, int idx, bool flag){
            x = (idx + 1.5) * tilesize;
            y = (idy + 2.5) * tilesize;
            if (flag && !game->grid[idx + idy * ncols].flagged){
                /* std::cout << "flag " << y << " " << x << std::endl; */
                game->right_click(x, y);
            }else if (!flag)
                game->left_click(x, y);
        }

        /* void step(Minesweeper & game){ */
        /*     read_board(); */
        /*     compute(); */
        /*     //click_tile(); */
        /* } */

        void compute(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    if (board_vals[row][col] > 0){
                        basic_1s_rule(row, col);
                        basic_1s_rule2(row, col);
                    }
                }
            }
        }


    private:
        void print_vals(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    std::cout << std::setw(2) << board_vals[row][col] << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        void basic_1s_rule(int i, int j){
            int n = 0;
            bool oU = false, oD = false,  oL =false, oR = false;
            oU = (i == 0);
            oD = (i == nrows-1);
            oL = (j == 0);
            oR = (j == ncols-1);

            if(!oU && board_vals[i-1][j]<0) n++;
            if(!oD && board_vals[i+1][j]<0) n++;
            if(!oL && board_vals[i][j-1]<0) n++;
            if(!oR && board_vals[i][j+1]<0) n++;
            
            if(!oU && !oL && board_vals[i-1][j-1]<0) n++;
            if(!oU && !oR && board_vals[i-1][j+1]<0) n++;
            if(!oD && !oL && board_vals[i+1][j-1]<0) n++;
            if(!oD && !oR && board_vals[i+1][j+1]<0) n++; 
            
            if(n == board_vals[i][j]){
                /* std::cout << i << " " << j << std::endl; */
                if(!oU && board_vals[i-1][j]==-1) click_tile(i-1,j,true);
                if(!oD && board_vals[i+1][j]==-1) click_tile(i+1,j,true);
                if(!oL && board_vals[i][j-1]==-1) click_tile(i,j-1,true);
                if(!oR && board_vals[i][j+1]==-1) click_tile(i,j+1,true); 
                if(!oU && !oL && board_vals[i-1][j-1]==-1) click_tile(i-1,j-1,true);
                if(!oU && !oR && board_vals[i-1][j+1]==-1) click_tile(i-1,j+1,true);
                if(!oD && !oL && board_vals[i+1][j-1]==-1) click_tile(i+1,j-1,true);
                if(!oD && !oR && board_vals[i+1][j+1]==-1) click_tile(i+1,j+1,true);
            }
        }
        
        void basic_1s_rule2(int i, int j){
            int n = 0;
            bool oU = false, oD = false,  oL =false, oR = false;
            oU = (i == 0);
            oD = (i == nrows-1);
            oL = (j == 0);
            oR = (j == ncols-1);

            if(!oU && board_vals[i-1][j]==-2) n++;
            if(!oD && board_vals[i+1][j]==-2) n++;
            if(!oL && board_vals[i][j-1]==-2) n++;
            if(!oR && board_vals[i][j+1]==-2) n++;
            
            if(!oU && !oL && board_vals[i-1][j-1]==-2) n++;
            if(!oU && !oR && board_vals[i-1][j+1]==-2) n++;
            if(!oD && !oL && board_vals[i+1][j-1]==-2) n++;
            if(!oD && !oR && board_vals[i+1][j+1]==-2) n++; 
            
            if(n == board_vals[i][j]){
                /* std::cout << i << " " << j << std::endl; */
                if(!oU && board_vals[i-1][j]==-1) click_tile(i-1,j,false);
                if(!oD && board_vals[i+1][j]==-1) click_tile(i+1,j,false);
                if(!oL && board_vals[i][j-1]==-1) click_tile(i,j-1,false);
                if(!oR && board_vals[i][j+1]==-1) click_tile(i,j+1,false); 
                if(!oU && !oL && board_vals[i-1][j-1]==-1) click_tile(i-1,j-1,false);
                if(!oU && !oR && board_vals[i-1][j+1]==-1) click_tile(i-1,j+1,false);
                if(!oD && !oL && board_vals[i+1][j-1]==-1) click_tile(i+1,j-1,false);
                if(!oD && !oR && board_vals[i+1][j+1]==-1) click_tile(i+1,j+1,false);
            }
        }
};

#endif
