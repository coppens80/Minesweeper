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
        std::vector<std::vector<int>> solutions, Mines, NoMines;
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
                        ++num_flags;
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

        void tank_solver(void){
            get_hidden_border();

            Mines = std::vector<std::vector<int>>(nrows, std::vector<int>(ncols,0));
            NoMines = std::vector<std::vector<int>>(nrows, std::vector<int>(ncols,0));
            for (int i=0; i<nrows; i++)
                for (int j=0; j<ncols; j++){
                    if(board[i][j].flagged) Mines[i][j] = 1;
                    if(board[i][j].val >= 0) NoMines[i][j] = 1;
                }

            //for (int i=0; i<int(border.size()); i++){
            for (int i=0; i<1; i++){
                std::cout << "Border[" << i << "]: (" << border[i].size() << ") \n"; 
                for(auto &x : border[i])
                    std::cout <<  x->row << " " << x->col << std::endl;
                
                solutions.clear();
                tank_recurse(border[i], 0, i);
                
                std::vector<float> result = std::vector<float>(border[i].size(),0);
                for (int j=0; j<solutions.size(); j++){
                    std::cout << "Solution[" << j << "]: (" << solutions[j].size() << ") \n"; 
                    for(int x=0; x<solutions[j].size(); x++){
                        std::cout << solutions[j][x] << " ";
                        result[x] += float(solutions[j][x]);
                    }
                    std::cout << std::endl;
                }
                std::cout << "Result: \n"; 
                for(int x=0; x<result.size(); x++){
                    std::cout << "(" << border[i][x]->row << "," << 
                        border[i][x]->col << ") " <<
                        result[x]/solutions.size() << std::endl;
                }
                std::cout << std::endl;
            }
            std::cout << "===================\n"; 

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
                unsigned int prev_size = border[i].size();
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
        }

        void tank_recurse(std::vector<Tile*> border_region, unsigned int k, int N){
            int flag_count = 0;
            // Check all tiles to see if current mine and empty tile arrangement makes sense
            for (int i=0; i<nrows; i++){
                for (int j=0; j<ncols; j++){
                    if(Mines[i][j])
                        flag_count++;

                    if (board[i][j].hidden || board[i][j].flagged)
                        continue;
                    
                    int mine_count = 0;
                    int free_count = 0;
                    for (auto& x : board[i][j].neighbours){
                        if(Mines[x->row][x->col]) mine_count++;
                        if(NoMines[x->row][x->col]) free_count++;
                    }
                    if (mine_count > board[i][j].val){
                        //std::cout << "Failed rule #1\n";
                        return; //solution fails: too many mines
                    }
                    
                    int num_neighbours = board[i][j].neighbours.size(); 
                    if (num_neighbours - free_count < board[i][j].val){
                        //std::cout << "Failed rule #2\n";
                        return; // solution fails: too many free tiles(non-mines)
                    }
                }
            }

            if (flag_count > num_mines){
                //std::cout << "Failed rule #3\n";
                return;
            }

            if (k == border_region.size()){
                /* if(flag_count < num_mines){ */
                /*     std::cout << "Failed rule #4\n"; */
                /*     return; //solution fails: not enough mines */
                /* } */

                // Add to solutions
                std::vector<int> solution; //placeholder
                for(auto &x : border_region)
                    solution.push_back(Mines[x->row][x->col]);
                solutions.push_back(solution);

                //std::cout << "Solution found!\n";
                return;
            }

            int idx = border_region[k]->col;
            int idy = border_region[k]->row;

            Mines[idy][idx] = true;
            tank_recurse(border_region, k+1, N);
            Mines[idy][idx] = false;
            
            NoMines[idy][idx] = true;
            tank_recurse(border_region, k+1, N);
            NoMines[idy][idx] = false;

        }

};

#endif
