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
        int score = 1;
    private:
        int ncols;
        int nrows;
        int num_mines;
        int num_flags;
        bool game_over = false;
        std::string gamemode;
        GridSquare cell;
        std::vector<GridSquare> grid;
        std::mt19937 generator;
        std::random_device rd;
        sf::Text score_display;
        sf::Font font;
        sf::Clock game_clock;

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
            window = new sf::RenderWindow(sf::VideoMode(cell.box_size*(ncols+2), cell.box_size*(nrows+2)), "Minesweepy");
        }

        void set_board(void){
            game_clock.restart();
            num_flags = num_mines;
            grid.reserve(ncols * nrows);
            create_tiles();
            set_mines();
            set_neighbour_values();
            setup_score_display();
        }

        void reset(void){
            grid.clear();
            game_over = false;
            set_board();
        }

        void draw_board(void){
            if (!game_over)
                score = int(game_clock.getElapsedTime().asSeconds());
            score_display.setString(std::to_string(score).c_str());
            window->clear();
            window->draw(score_display);
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
                        end_game();
                    else if (square.val == 0)
                        reveal_neighbours(idx);
                    break;
                }
                ++idx;
            }
        }
        
        void right_click(const sf::Event& event) {
            for(auto& square : grid)
                square.flag(event, num_flags);
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
                    if (grid[x+y*ncols].val == 0)
                        reveal_neighbours(x + y * ncols);
                }
            }
        }

        void end_game(void){
            game_over = true;
            for(auto& square : grid)
                if(square.is_mine)
                    square.reveal();
            printf("Game Over :(\n");
        }
        
        void setup_score_display(void){
            if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
                std::cout << "Font not loaded" << std::endl;
            score_display.setFont(font);
            score_display.setCharacterSize(20);
            score_display.setFillColor(sf::Color::White);
            score_display.setPosition(0, 0);
        }
};

#endif
