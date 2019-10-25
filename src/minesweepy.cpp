#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "gameboard.h"

int main(int argc, char** argv){

    gameboard board("Easy");

    //sf::Clock clock;
    sf::Event event;

    //board.window.setKeyRepeatEnabled(False);

    while (board.window->isOpen()) {
        while (board.window->pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                board.window->close();
            
            if (event.type == sf::Event::MouseButtonPressed){
                board.leftClick(event);
                if (event.mouseButton.button == sf::Mouse::Left){
                    std::cout << "the left button was pressed" << std::endl;
                }
                if (event.mouseButton.button == sf::Mouse::Right){
                    std::cout << "the right button was pressed" << std::endl;
                }
                std::cout << "mouse x: " << event.mouseButton.x << std::endl;
                std::cout << "mouse y: " << event.mouseButton.y << std::endl;
            }
        }

        //std::cout << "Elapsed time in ms: " << clock.getElapsedTime().asMicroseconds() << std::endl;
        //clock.restart();
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            board.window->close();

        board.drawBoard();
    }

    return 0;
}

