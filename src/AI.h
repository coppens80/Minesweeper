//SRC: J.Coppens 2019

#ifndef SWEEPERAI
#define SWEEPERAI

#include <vector>
#include "minesweeper.h"
#include "tile.h"
#include <iomanip>

struct Tile {
    int val = -1;
    bool hidden = true;
    bool isBorder = false;
    bool flagged = false;
};

class MinesweeperAI {
    // Data
    public:
        int ncols;
        int nrows;
        int num_mines;
        int num_flags;
        float tilesize; 
    private:
        std::vector<std::vector<Tile>> board;
        float x, y;
        Minesweeper *game;
        Tile default_tile;

    // Methods
    public:
        MinesweeperAI (Minesweeper* gameref){
            game = gameref;
            nrows = game->nrows;
            ncols = game->ncols;
            num_mines = game->num_mines;
            num_flags = game->num_flags;
            tilesize = game->s;
            board = std::vector<std::vector<Tile>>(nrows, std::vector<Tile>(ncols,default_tile));
        }

        void read_board(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    board[row][col] = default_tile;
                    if (game->grid[col + row * ncols].is_clicked){
                        board[row][col].val = game->grid[col + row * ncols].val;
                        board[row][col].hidden = false;
                    }
                    else if (game->grid[col + row * ncols].flagged){
                        board[row][col].val = -2;
                        board[row][col].flagged = true;
                        board[row][col].hidden = false;
                    }
                }
            }
            get_border_tiles();
            
            /* print_vals(); */
            /* print_border(); */
        }

        /* void step(Minesweeper & game){ */
        /*     read_board(); */
        /*     compute(); */
        /*     //click_tile(); */
        /* } */

        void compute(void){
            int passed = 0;
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    if (!board[row][col].hidden && board[row][col].isBorder){
                        passed += basic_hidden_neighbour_rule(row, col);
                        passed += basic_flag_rule(row, col);
                    }
                }
            }
            
            if (passed == 0)
                std::cout << "All basic tests failed\n";
        }


    private:
        void click_tile(int idy, int idx){
            x = (idx + 1.5) * tilesize;
            y = (idy + 2.5) * tilesize;
            game->left_click(x, y);
        }
        
        void flag_tile(int idy, int idx){
            x = (idx + 1.5) * tilesize;
            y = (idy + 2.5) * tilesize;
            if (!board[idy][idx].flagged){
                game->right_click(x, y);
                board[idy][idx].flagged = true;
            }
        }

        void print_vals(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    std::cout << std::setw(2) << board[row][col].val << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }

        void print_border(void){
            for (int row=0; row<nrows; row++)
                for (int col=0; col<ncols; col++)
                    if (board[row][col].isBorder)
                        std::cout << row << " " << col << " " << board[row][col].val << std::endl;
        }

        void get_border_tiles(void){
            bool oU, oD, oL, oR;
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    if (board[i][j].flagged || board[i][j].hidden)
                        continue;
                    oU = false, oD = false,  oL =false, oR = false;
                    oU = (i == 0);
                    oD = (i == nrows-1);
                    oL = (j == 0);
                    oR = (j == ncols-1);
                    
                    if(!oU && board[i-1][j].hidden) board[i][j].isBorder = true;
                    if(!oD && board[i+1][j].hidden) board[i][j].isBorder = true;
                    if(!oL && board[i][j-1].hidden) board[i][j].isBorder = true;
                    if(!oR && board[i][j+1].hidden) board[i][j].isBorder = true;
                    
                    if(!oU && !oL && board[i-1][j-1].hidden) board[i][j].isBorder = true;
                    if(!oU && !oR && board[i-1][j+1].hidden) board[i][j].isBorder = true;
                    if(!oD && !oL && board[i+1][j-1].hidden) board[i][j].isBorder = true;
                    if(!oD && !oR && board[i+1][j+1].hidden) board[i][j].isBorder = true; 
                }
            }
        }

        int basic_hidden_neighbour_rule(int i, int j){
            int n = 0;
            bool oU = false, oD = false,  oL =false, oR = false;
            oU = (i == 0);
            oD = (i == nrows-1);
            oL = (j == 0);
            oR = (j == ncols-1);
            
            if(!oU && (board[i-1][j].hidden || board[i-1][j].flagged)) n++;
            if(!oD && (board[i+1][j].hidden || board[i+1][j].flagged)) n++;
            if(!oL && (board[i][j-1].hidden || board[i][j-1].flagged)) n++;
            if(!oR && (board[i][j+1].hidden || board[i][j+1].flagged)) n++;
            
            if(!oU && !oL && (board[i-1][j-1].hidden || board[i-1][j-1].flagged)) n++;
            if(!oU && !oR && (board[i-1][j+1].hidden || board[i-1][j+1].flagged)) n++;
            if(!oD && !oL && (board[i+1][j-1].hidden || board[i+1][j-1].flagged)) n++;
            if(!oD && !oR && (board[i+1][j+1].hidden || board[i+1][j+1].flagged)) n++; 
            
            if(n == board[i][j].val){
                if(!oU && board[i-1][j].hidden) flag_tile(i-1,j);
                if(!oD && board[i+1][j].hidden) flag_tile(i+1,j);
                if(!oL && board[i][j-1].hidden) flag_tile(i,j-1);
                if(!oR && board[i][j+1].hidden) flag_tile(i,j+1); 
                if(!oU && !oL && board[i-1][j-1].hidden) flag_tile(i-1,j-1);
                if(!oU && !oR && board[i-1][j+1].hidden) flag_tile(i-1,j+1);
                if(!oD && !oL && board[i+1][j-1].hidden) flag_tile(i+1,j-1);
                if(!oD && !oR && board[i+1][j+1].hidden) flag_tile(i+1,j+1);
                return 1;
            }else
                return 0;
        }
        
        int basic_flag_rule(int i, int j){
            int n = 0;
            bool oU = false, oD = false,  oL =false, oR = false;
            oU = (i == 0);
            oD = (i == nrows-1);
            oL = (j == 0);
            oR = (j == ncols-1);

            if(!oU && board[i-1][j].flagged) n++;
            if(!oD && board[i+1][j].flagged) n++;
            if(!oL && board[i][j-1].flagged) n++;
            if(!oR && board[i][j+1].flagged) n++;
            
            if(!oU && !oL && board[i-1][j-1].flagged) n++;
            if(!oU && !oR && board[i-1][j+1].flagged) n++;
            if(!oD && !oL && board[i+1][j-1].flagged) n++;
            if(!oD && !oR && board[i+1][j+1].flagged) n++; 
            
            if(n == board[i][j].val){
                if(!oU && board[i-1][j].hidden) click_tile(i-1,j);
                if(!oD && board[i+1][j].hidden) click_tile(i+1,j);
                if(!oL && board[i][j-1].hidden) click_tile(i,j-1);
                if(!oR && board[i][j+1].hidden) click_tile(i,j+1); 
                if(!oU && !oL && board[i-1][j-1].hidden) click_tile(i-1,j-1);
                if(!oU && !oR && board[i-1][j+1].hidden) click_tile(i-1,j+1);
                if(!oD && !oL && board[i+1][j-1].hidden) click_tile(i+1,j-1);
                if(!oD && !oR && board[i+1][j+1].hidden) click_tile(i+1,j+1);
                return 1;
            }else
                return 0;
        }


};

#endif
