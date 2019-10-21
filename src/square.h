#ifndef SQUARE
#define SQUARE

#include <iostream>
#include <sstream>

class GridSquare : public sf::Drawable {
    public:    
        float box_size = 25.f;
        int val;
    
    public:
        GridSquare(void){
            square.setSize(sf::Vector2f(box_size, box_size));
            square.setFillColor(sf::Color::White);
            square.setOutlineThickness(2.f);
            square.setOutlineColor(sf::Color::Black);
            setValue(0);
            
            if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
                std::cout << "Font not loaded" << std::endl;
            text.setFont(font);
            text.setString(std::to_string(val));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
        }
    
        void setPosition(float x, float y){
            square.setPosition(x, y);
            text.setPosition(x+6, y);
        }
        void setValue(int i){
            val = i;
            text.setString(std::to_string(val).c_str());
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            target.draw(square);
            target.draw(text);
        }
    
    private:
        sf::RectangleShape square;
        sf::Text text;
        sf::Font font;
};

#endif
