#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>

int main(int argc, char** argv){

    sf::RenderWindow window(sf::VideoMode(640, 480), "Minesweepy");
    /* sf::CircleShape shape(100.f); */
    /* shape.setFillColor(sf::Color::Green); */
    /* shape.setOrigin(shape.getLocalBounds().width/2.0f, shape.getLocalBounds().height/2.0f); */

    int score = 12345;
    sf::Text text;
    std::stringstream ss;
    ss << score;

    sf::Font font;
    if (!font.loadFromFile("../dep/Fonts/Arial/Arial.ttf"))
        std::cout << "Font not loaded" << std::endl;
    text.setFont(font);

    //text.setString("Hello world");
    text.setString(ss.str().c_str());
    text.setCharacterSize(12);
    text.setFillColor(sf::Color::Red);

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

        window.clear(sf::Color::Blue);
        window.draw(text);
        /* window.draw(shape); */
        window.display();
    }

    return 0;
}

