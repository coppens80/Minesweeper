#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "gameboard.h"

int main(int argc, char** argv){

    gameboard board("Medium");

    sf::RenderWindow window(sf::VideoMode(640, 480), "Minesweepy");
    sf::RectangleShape cell(sf::Vector2f(25.f, 25.f));
    cell.setFillColor(sf::Color::White);
    cell.setOutlineThickness(2.f);
    cell.setOutlineColor(sf::Color::Blue);
    /* cell.setPosition(100., 100.); */

    int score = 12345;
    sf::Text text;
    std::stringstream ss;
    ss << score;

    sf::Font font;
    if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
        std::cout << "Font not loaded" << std::endl;
    text.setFont(font);

    text.setString(ss.str().c_str());
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::Black);
    //text.setPosition(100.f, 100.f);

    //sf::Clock clock;
    sf::Event event;

    //window.setKeyRepeatEnabled(False);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::TextEntered){
                if (event.text.unicode < 128){
                    std::cout << static_cast<char>(event.text.unicode) << std::endl;
                    text.setString(static_cast<char>(event.key.code));
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed){
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
            window.close();

        window.clear();
        window.draw(cell);
        window.draw(text);
        window.display();
    }

    return 0;
}

