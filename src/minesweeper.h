//SRC: J.Coppens 2019

#ifndef BOARD
#define BOARD

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>
#include "tile.h"

class Minesweeper {
    // Data
    public:
        std::pair<float,float> window_size;
        int score;
        bool game_over = false;
        bool game_won = false;
        int ncols;
        int nrows;
        int num_mines;
        int num_flags;
        float s;
        std::vector<GameTile> grid;
    private:
        int tiles_cleared;
        std::string gamemode;
        GameTile single_tile;
        sf::Text score_display, flag_display;
        sf::Font font;
        sf::Clock game_clock;
        int idx_x, idx_y, idx;

    // Methods
    public:
        Minesweeper(std::string mode) : gamemode(mode){
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

            s = single_tile.size;
            window_size = std::make_pair(s*(ncols+2), s*(nrows+3));
            setup_score_display();
            set_board();
        }

        void reset(void){
            grid.clear();
            game_over = false;
            game_won = false;
            set_board();
            int m = 0;
            for(auto& tile : grid)
                if (tile.is_mine)
                    m++;
        }

        void draw_board(sf::RenderWindow &window){
            if (!game_over)
                score = int(game_clock.getElapsedTime().asSeconds());
            score_display.setString(std::to_string(score).c_str());
            flag_display.setString(std::to_string(num_flags).c_str());
            
            window.clear();
            window.draw(score_display);
            window.draw(flag_display);
            for(auto& tile : grid)
                window.draw(tile);
            window.display();
        }

        void left_click(float mouse_x, float mouse_y) {
            idx_x = int(mouse_x/s + 1) - 2;
            idx_y = int(mouse_y/s + 1) - 3;
            if (idx_x < 0 || idx_x >= ncols || idx_y < 0 || idx_y >= nrows)
                return;
            idx = idx_x + idx_y * ncols;
            
            if(grid[idx].flagged || grid[idx].is_clicked)
                return;

            grid[idx].click();
            
            if(grid[idx].is_mine)
                end_game();
            else
                ++tiles_cleared;
            
            if (grid[idx].val == 0)
                reveal_neighbours(idx);
            
            if (tiles_cleared == nrows * ncols - num_mines)
                win_game();
        }
        
        void right_click(float mouse_x, float mouse_y) {
            idx_x = int(mouse_x/s + 1) - 2;
            idx_y = int(mouse_y/s + 1) - 3;
            if (idx_x < 0 || idx_x >= ncols || idx_y < 0 || idx_y >= nrows)
                return;
            idx = idx_x + idx_y * ncols;
            
            if (grid[idx].is_clicked)
                return;
            grid[idx].flag(num_flags);
        }

    private:
        void set_board(void){
            std::cout << "Game mode: " << gamemode << std::endl;
            num_flags = num_mines;
            tiles_cleared = 0;
            grid.reserve(ncols * nrows);
            create_tiles();
            set_mines();
            set_neighbour_values();
            game_clock.restart();
        }

        void create_tiles(void){
            for(int row=0; row<nrows; row++){
                for(int col=0; col<ncols; col++){
                    grid.push_back(single_tile);
                    grid[col + row * ncols].set_position((col+1)*s, (row+2)*s);
                }
            }
        }

        void set_mines(void){
            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<int> mine_idx(0, ncols * nrows - 1);
            int count = 0, idx;
            while (count < num_mines){
                idx = mine_idx(generator);
                if (grid[idx].is_mine)
                    continue;
                else{
                    grid[idx].create_mine();
                    count++;
                }
            }
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
                    ++tiles_cleared;
                    if (grid[x+y*ncols].val == 0)
                        reveal_neighbours(x + y * ncols);
                }
            }
        }

        void end_game(void){
            game_over = true;
            game_won = false;
            for(auto& tile : grid)
                if(tile.is_mine)
                    tile.reveal();
            std::cout << "Game Over :(" << std::endl;
            std::cout << "Press R to restart" << std::endl;
        }

        void win_game(void){
            game_over = true;
            game_won = true;
            std::cout << "You win!" << std::endl;
            std::cout << "Score: " << score << std::endl;
            std::cout << "Press R to restart" << std::endl;
        }
        
        void setup_score_display(void){
            if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
                std::cout << "Font not loaded" << std::endl;
            score_display.setFont(font);
            score_display.setCharacterSize(20);
            score_display.setFillColor(sf::Color::White);
            score_display.setPosition(s*(ncols-2), 12);
            flag_display.setFont(font);
            flag_display.setCharacterSize(20);
            flag_display.setFillColor(sf::Color::White);
            flag_display.setPosition(s*3, 12);
        }
};

#endif
