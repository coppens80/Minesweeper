#include <SFML/Graphics.hpp>
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        std::cout << "Elapsed time in ms: " << clock.getElapsedTime().asMicroseconds() << std::endl;
        clock.restart();

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
}

