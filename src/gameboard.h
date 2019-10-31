#ifndef BOARD
#define BOARD

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>
#include "square.h"

class Gameboard {
    // Data
    public:
        sf::RenderWindow *window;
        int game_time = 1;
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
        sf::Text game_score;
        sf::Font font;

    // Methods
    public:
        Gameboard(std::string mode) : gamemode(mode){
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
        }

        void set_board(void){
            grid.reserve(num_squares);
            create_tiles();
            set_mines();
            set_neighbour_values();
            setup_game_score();
        }

        void reset(void){
            grid.clear();
            set_board();
        }

        void draw_board(void){
            game_score.setString(std::to_string(game_time).c_str());
            window->clear();
            window->draw(game_score);
            for(auto& square : grid)
                window->draw(square);
            window->display();
        }

        void left_click(const sf::Event& event) {
            int idx = 0;
            for(auto& square : grid){
                if(square.flagged || square.is_clicked){
                    ++idx;
                    continue;
                }
                if(square.click(event)){
                    if(square.is_mine)
                        game_over();
                    else if (square.val == 0)
                        reveal_neighbours(idx);
                    break;
                }
                ++idx;
            }
        }
        
        void right_click(const sf::Event& event) {
            for(auto& square : grid)
                square.flag(event);
        }

    private:
        void create_tiles(void){
            for(int row=0; row<nrows; row++){
                for(int col=0; col<ncols; col++){
                    grid.push_back(cell);
                    grid[col + row * ncols].set_position((col+1)*cell.box_size, (row+1)*cell.box_size);
                }
            }
        }

        void set_mines(void){
            generator = std::mt19937(rd());
            std::uniform_int_distribution<int> mine_idx(0, num_squares);
            for (int i=0; i<num_mines; i++)
                grid[mine_idx(generator)].create_mine();
        }

        void set_neighbour_values(void){
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
                    grid[col + row * ncols].set_value(nearby_mines);
                }
            }
        }

        void reveal_neighbours(int i){
            int col = i % ncols;
            int row = (i - i % ncols) / ncols;
            
            for (int i=0; i<9; i++){
                int x = col + i/3 - 1;
                int y = row + i%3 - 1;
                
                if(x < 0 || x >= ncols || y < 0 || y >= nrows)
                    continue;
                if(x == col && y == row)
                    continue;
                
                if(!grid[x+y*ncols].is_clicked && !grid[x+y*ncols].flagged && !grid[x+y*ncols].is_mine){
                    grid[x+y*ncols].reveal(); 
                    if (grid[x+y*ncols].val == 0)
                        reveal_neighbours(x + y * ncols);
                }
            }
        }

        void game_over(void){
            for(auto& square : grid)
                if(square.is_mine)
                    square.reveal();
            printf("Game Over :(\n");
        }
        
        void setup_game_score(void){
            if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
                std::cout << "Font not loaded" << std::endl;
            game_score.setFont(font);
            game_score.setCharacterSize(20);
            game_score.setFillColor(sf::Color::White);
            game_score.setPosition(0, 0);
        }
};

#endif
