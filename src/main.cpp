//SRC: J.Coppens 2019

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "minesweeper.h"
#include "AI.h"

int main(int argc, char** argv){
    
    Minesweeper game("Easy");
    MinesweeperAI solver(game);
    auto [width, height] = game.window_size;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "My Shitty Minesweeper");
    window.setKeyRepeatEnabled(false);
    
    sf::Event event;
    sf::Clock timer;
    int n = 0;

    while (window.isOpen()) {
         
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed && !game.game_over){
                if (event.mouseButton.button == sf::Mouse::Left)
                    game.left_click(event.mouseButton.x, event.mouseButton.y);
                if (event.mouseButton.button == sf::Mouse::Right)
                    game.right_click(event.mouseButton.x, event.mouseButton.y);
            }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::H){
                    solver.read_board(game.grid);
                }
                if (event.key.code == sf::Keyboard::R){
                    printf("reset\n");
                    game.reset();
                    std::cout << "Average frame time: " << timer.getElapsedTime().asSeconds() * 1000 / n << std::endl;
                    timer.restart();
                }
            }
        
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            window.close();

        game.draw_board(window);
        n++;
    }

    std::cout << "Average frame time: " << timer.getElapsedTime().asSeconds() * 1000 / n << std::endl;

    return 0;
}

