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
    private:
        int ncols;
        int nrows;
        int num_mines;
        int num_flags;
        int tiles_cleared;
        std::string gamemode;
        GameTile single_tile;
        std::vector<GameTile> grid;
        sf::Text score_display, flag_display;
        sf::Font font;
        sf::Clock game_clock;

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

            window_size = std::make_pair(single_tile.box_size*(ncols+2), single_tile.box_size*(nrows+3));
            set_board();
        }

        void set_board(void){
            std::cout << "Game mode: " << gamemode << std::endl;
            num_flags = num_mines;
            tiles_cleared = 0;
            grid.reserve(ncols * nrows);
            create_tiles();
            set_mines();
            set_neighbour_values();
            setup_score_display();
            game_clock.restart();
        }

        void reset(void){
            grid.clear();
            game_over = false;
            set_board();
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

        void left_click(const sf::Event& event) {
            int idx = 0;
            for(auto& tile : grid){
                if(tile.flagged || tile.is_clicked){
                    ++idx;
                    continue;
                }
                if(tile.click(event)){
                    if(tile.is_mine)
                        end_game();
                    else
                        ++tiles_cleared;
                    if (tile.val == 0)
                        reveal_neighbours(idx);
                    break;
                }
                ++idx;
            }
            if (tiles_cleared == nrows * ncols - num_mines)
                win_game();
        }
        
        void right_click(const sf::Event& event) {
            for(auto& tile : grid)
                tile.flag(event, num_flags);
        }

    private:
        void create_tiles(void){
            for(int row=0; row<nrows; row++){
                for(int col=0; col<ncols; col++){
                    grid.push_back(single_tile);
                    grid[col + row * ncols].set_position((col+1)*single_tile.box_size, (row+2)*single_tile.box_size);
                }
            }
        }

        void set_mines(void){
            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<int> mine_idx(0, ncols * nrows);
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
                    ++tiles_cleared;
                    if (grid[x+y*ncols].val == 0)
                        reveal_neighbours(x + y * ncols);
                }
            }
        }

        void end_game(void){
            game_over = true;
            for(auto& tile : grid)
                if(tile.is_mine)
                    tile.reveal();
            std::cout << "Game Over :(" << std::endl;
            std::cout << "Press R to restart" << std::endl;
        }

        void win_game(void){
            game_over = true;
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
            score_display.setPosition(single_tile.box_size*(ncols-2), 12);
            flag_display.setFont(font);
            flag_display.setCharacterSize(20);
            flag_display.setFillColor(sf::Color::White);
            flag_display.setPosition(single_tile.box_size*3, 12);
        }
};

#endif
