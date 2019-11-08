#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "minesweeper.h"

int main(int argc, char** argv){
    
    Minesweeper game("Easy");
    auto [width, height] = game.window_size;

    sf::RenderWindow window(sf::VideoMode(width, height), "My Shitty Minesweeper");
    window.setKeyRepeatEnabled(false);
    
    sf::Event event;

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed && !game.game_over){
                if (event.mouseButton.button == sf::Mouse::Left)
                    game.left_click(event);
                if (event.mouseButton.button == sf::Mouse::Right)
                    game.right_click(event);
            }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::R){
                    printf("reset\n");
                    game.reset();
                }
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            window.close();
        
        game.draw_board(window);
    }

    return 0;
}

