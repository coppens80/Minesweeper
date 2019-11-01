#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "gameboard.h"

int main(int argc, char** argv){

    Gameboard board("Easy");
    board.set_board();
    board.window->setKeyRepeatEnabled(false);

    sf::Event event;

    while (board.window->isOpen()) {
        while (board.window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                board.window->close();
            
            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left)
                    board.left_click(event);
                if (event.mouseButton.button == sf::Mouse::Right)
                    board.right_click(event);
            }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::R)
                    board.reset();
            }
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            board.window->close();
        
        board.draw_board();
    }

    return 0;
}

