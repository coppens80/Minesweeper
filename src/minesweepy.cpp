#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "gameboard.h"

int main(int argc, char** argv){

    Gameboard board("Hard");
    board.set_board();

    //sf::Clock clock;
    sf::Event event;

    //board.window.setKeyRepeatEnabled(False);

    while (board.window->isOpen()) {
        while (board.window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                board.window->close();
            
            if (event.type == sf::Event::MouseButtonPressed){
                if (event.mouseButton.button == sf::Mouse::Left){
                    board.left_click(event);
                }
                if (event.mouseButton.button == sf::Mouse::Right){
                    board.right_click(event);
                }
            }
        }

        //std::cout << "Elapsed time in ms: " << clock.getElapsedTime().asMicroseconds() << std::endl;
        //clock.restart();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            board.window->close();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::R) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            board.reset();

        board.draw_board();
    }

    return 0;
}

