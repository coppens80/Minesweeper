//SRC: J.Coppens 2019

#ifndef SWEEPERAI
#define SWEEPERAI

#include <vector>
#include "minesweeper.h"
#include "tile.h"
#include <iomanip>
#include <stdlib.h> //abs

struct Tile {
    int row;
    int col;
    int val = -1;
    bool hidden = true;
    bool isBorderNum = false;
    bool flagged = false;
    std::vector<Tile*> neighbours;
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
        std::vector<std::vector<Tile*>> border;
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
            //num_flags = game->num_flags;
            tilesize = game->s;
            board = std::vector<std::vector<Tile>>(nrows, std::vector<Tile>(ncols,default_tile));
        }

        void read_board(void){
            for (int row=0; row<nrows; row++){
                for (int col=0; col<ncols; col++){
                    board[row][col] = default_tile;
                    board[row][col].row = row;
                    board[row][col].col = col;
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
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    bool oU = false, oD = false,  oL =false, oR = false;
                    oU = (i == 0);
                    oD = (i == nrows-1);
                    oL = (j == 0);
                    oR = (j == ncols-1);
                    
                    if(!oU) board[i][j].neighbours.push_back(&board[i-1][j]);
                    if(!oD) board[i][j].neighbours.push_back(&board[i+1][j]);
                    if(!oL) board[i][j].neighbours.push_back(&board[i][j-1]);
                    if(!oR) board[i][j].neighbours.push_back(&board[i][j+1]);
                    
                    if(!oU && !oL) board[i][j].neighbours.push_back(&board[i-1][j-1]);
                    if(!oU && !oR) board[i][j].neighbours.push_back(&board[i-1][j+1]);
                    if(!oD && !oL) board[i][j].neighbours.push_back(&board[i+1][j-1]);
                    if(!oD && !oR) board[i][j].neighbours.push_back(&board[i+1][j+1]); 
                }
            }

            find_border_numbers();
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
                    if (!board[row][col].hidden && board[row][col].isBorderNum){
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
            board[idy][idx].hidden = false;
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

        void find_border_numbers(void){
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    if (board[i][j].flagged || board[i][j].hidden)
                        continue;
                    
                    for (auto& x : board[i][j].neighbours){
                        if(x->hidden){
                            board[i][j].isBorderNum = true;
                            break;
                        }
                    }
                }
            }
        }

        int basic_hidden_neighbour_rule(int i, int j){
            int n = 0;
            for (auto& x : board[i][j].neighbours)
                if(x->hidden || x->flagged) n++;
            
            if(n == board[i][j].val){
                for (auto& x : board[i][j].neighbours)
                    if(x->hidden) flag_tile(x->row,x->col);
                return 1;
            }
            return 0;
        }
        
        int basic_flag_rule(int i, int j){
            int n = 0;
            for (auto& x : board[i][j].neighbours)
                if(x->flagged) n++;
            
            if(n == board[i][j].val){
                for (auto& x : board[i][j].neighbours)
                    if(x->hidden) click_tile(x->row,x->col);
                return 1;
            }
            return 0;
        }

        void get_hidden_border(void){
            border.clear(); 
            std::vector<Tile*> queue;
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    if (board[i][j].flagged || !board[i][j].hidden)
                        continue;
                    for (auto& x : board[i][j].neighbours){
                        if(x->hidden == false && x->flagged == false){
                            queue.push_back(&board[i][j]);
                            break;
                        }
                    }
                }
            }
            border.push_back(std::vector<Tile*> (1,queue[0]));
            queue.erase(queue.begin());
            
            int i = 0;
            while(queue.begin() != queue.end()){
                int prev_size = border[i].size();
                for(auto &b : border[i]){
                    for(auto it=queue.begin(); it!=queue.end(); it++){
                        if(( abs((*it)->row - b->row) + abs((*it)->col -  b->col) ) == 1){
                            border[i].push_back(*it);
                            queue.erase(it);
                            break;
                        }
                    }
                }
                if (prev_size == border[i].size()){
                    border.push_back(std::vector<Tile*> (1,queue[0]));
                    queue.erase(queue.begin());
                    i++;
                }
            }
            
            for(int i=0; i<border.size(); i++){
                std::cout << "Border[" << i << "]: (" << border[i].size() << ") \n"; 
                for(auto &x : border[i])
                    std::cout << x << " " <<  x->row << " " << x->col << std::endl;
            }
            std::cout << "===================\n"; 
        }

        void tank_solver(void){
            get_hidden_border();

            //create solutions array...
            //create Mines array...
            //create EmptyTile array

            for (int i=0; i<border.size(); i++){
                tank_recurse(border[i], 0);
            }

        }

        void tank_recurse(std::vector<Tile*> border_region, int k){
            int flag_count = 0;
            // check if num mines and num empty makes sense for ALL tiles, if not, exit
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    if (board[i][j].hidden || board[i][j].flagged)
                        continue;
                    if(Mines[i][j])
                        flag_count++;
                    //count num mines around i,j
                    if (m > board[i][j].val)
                        return;

                    //count num free spaces
                    //
                    //if (numFree > 
                }
            }

            if (flag_count > num_mines)
                return;

            (if k == border_region.size()){
                if(flag_count < num_mines)
                    return;
                //make num mines boolean array (1=mine, 0=notmine)
                //add array to solutions
            }
            int idx = border_region[k]->col;
            int idy = border_region[k]->row;

            Mines[idx][idx] = true;
            tank_recurse(border_region, k++);
            Mines[idy][idx] = false;
            
            Empty[idx][idx] = true;
            tank_recurse(border_region, k++);
            Empty[idy][idx] = false;

        }

};

#endif
