//SRC: J.Coppens 2019

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
        std::vector<std::vector<int>> border;
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
            border.clear();
            get_border_tiles();
            
            /* print_vals(); */
            /* print_border(); */
        }

        void click_tile(int idy, int idx, bool flag){
            x = (idx + 1.5) * tilesize;
            y = (idy + 2.5) * tilesize;
            if (flag && !game->grid[idx + idy * ncols].flagged){
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
            int passed = 0;
            for (int i=0; i<int(border.size()); i++){
                int row = border[i][0];
                int col = border[i][1];
                if (board_vals[row][col] > 0){
                    passed += basic_hidden_neighbour_rule(row, col);
                    passed += basic_flag_rule(row, col);
                }
            }
            
            if (passed == 0)
                std::cout << "All basic tests failed\n";
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

        void print_border(void){
            for (int i=0; i<int(border.size()); i++)
                std::cout << border[i][0] << " " << border[i][1] << " " << border[i][2] << std::endl;
        }

        void get_border_tiles(void){
            bool oU, oD, oL, oR, isBorder;
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    if (board_vals[i][j] < 0)
                        continue;
                    oU = false, oD = false,  oL =false, oR = false;
                    oU = (i == 0);
                    oD = (i == nrows-1);
                    oL = (j == 0);
                    oR = (j == ncols-1);
                    isBorder = false;
                    
                    if(!oU && board_vals[i-1][j]==-1) isBorder = true;
                    if(!oD && board_vals[i+1][j]==-1) isBorder = true;
                    if(!oL && board_vals[i][j-1]==-1) isBorder = true;
                    if(!oR && board_vals[i][j+1]==-1) isBorder = true;
                    
                    if(!oU && !oL && board_vals[i-1][j-1]==-1) isBorder = true;
                    if(!oU && !oR && board_vals[i-1][j+1]==-1) isBorder = true;
                    if(!oD && !oL && board_vals[i+1][j-1]==-1) isBorder = true;
                    if(!oD && !oR && board_vals[i+1][j+1]==-1) isBorder = true; 
                    
                    if(isBorder)
                        border.push_back(std::vector<int> {i,j,board_vals[i][j]});
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

            if(!oU && board_vals[i-1][j]<0) n++;
            if(!oD && board_vals[i+1][j]<0) n++;
            if(!oL && board_vals[i][j-1]<0) n++;
            if(!oR && board_vals[i][j+1]<0) n++;
            
            if(!oU && !oL && board_vals[i-1][j-1]<0) n++;
            if(!oU && !oR && board_vals[i-1][j+1]<0) n++;
            if(!oD && !oL && board_vals[i+1][j-1]<0) n++;
            if(!oD && !oR && board_vals[i+1][j+1]<0) n++; 
            
            if(n == board_vals[i][j]){
                if(!oU && board_vals[i-1][j]==-1) click_tile(i-1,j,true);
                if(!oD && board_vals[i+1][j]==-1) click_tile(i+1,j,true);
                if(!oL && board_vals[i][j-1]==-1) click_tile(i,j-1,true);
                if(!oR && board_vals[i][j+1]==-1) click_tile(i,j+1,true); 
                if(!oU && !oL && board_vals[i-1][j-1]==-1) click_tile(i-1,j-1,true);
                if(!oU && !oR && board_vals[i-1][j+1]==-1) click_tile(i-1,j+1,true);
                if(!oD && !oL && board_vals[i+1][j-1]==-1) click_tile(i+1,j-1,true);
                if(!oD && !oR && board_vals[i+1][j+1]==-1) click_tile(i+1,j+1,true);
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

            if(!oU && board_vals[i-1][j]==-2) n++;
            if(!oD && board_vals[i+1][j]==-2) n++;
            if(!oL && board_vals[i][j-1]==-2) n++;
            if(!oR && board_vals[i][j+1]==-2) n++;
            
            if(!oU && !oL && board_vals[i-1][j-1]==-2) n++;
            if(!oU && !oR && board_vals[i-1][j+1]==-2) n++;
            if(!oD && !oL && board_vals[i+1][j-1]==-2) n++;
            if(!oD && !oR && board_vals[i+1][j+1]==-2) n++; 
            
            if(n == board_vals[i][j]){
                if(!oU && board_vals[i-1][j]==-1) click_tile(i-1,j,false);
                if(!oD && board_vals[i+1][j]==-1) click_tile(i+1,j,false);
                if(!oL && board_vals[i][j-1]==-1) click_tile(i,j-1,false);
                if(!oR && board_vals[i][j+1]==-1) click_tile(i,j+1,false); 
                if(!oU && !oL && board_vals[i-1][j-1]==-1) click_tile(i-1,j-1,false);
                if(!oU && !oR && board_vals[i-1][j+1]==-1) click_tile(i-1,j+1,false);
                if(!oD && !oL && board_vals[i+1][j-1]==-1) click_tile(i+1,j-1,false);
                if(!oD && !oR && board_vals[i+1][j+1]==-1) click_tile(i+1,j+1,false);
                return 1;
            }else
                return 0;
        }


};

#endif
