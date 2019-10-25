#ifndef SQUARE
#define SQUARE

#include <iostream>
#include <sstream>

class GridSquare : public sf::Drawable {
    public:    
        float box_size = 25.f;
        int val = 0;
        bool is_mine = false;
        bool flagged = false;
        bool is_clicked = false;
    
    public:
        GridSquare(void){
            square.setSize(sf::Vector2f(box_size, box_size));
            square.setFillColor(sf::Color(84, 84, 84, 255));
            square.setOutlineThickness(2.f);
            square.setOutlineColor(sf::Color::Black);
            
            set_up_text();
        }
    
        void setPosition(float x, float y){
            square.setPosition(x, y);
            text.setPosition(x+6, y);
        }
        
        void setValue(int i){
            val = i;
            if(val == 0)
                text.setString(" ");
            else
                text.setString(std::to_string(val).c_str());
        }

        void createMine(void){
            is_mine = true;
            text.setString('x');
            text.setFillColor(sf::Color::Red);
        }

        bool click(const sf::Event event) {
            if(square.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !flagged){
                is_clicked = true;
                square.setFillColor(sf::Color::White);
                if(is_mine) 
                    return true;
            }
            return false;
        }
        
        void flag(const sf::Event event) {
            if(square.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !is_clicked){
                if (!flagged){
                    flagged = true;
                    square.setFillColor(sf::Color::Blue);
                }else{
                    flagged = false;
                    square.setFillColor(sf::Color(84, 84, 84, 255));
                }
            }
        }

        void reveal(void){
            is_clicked = true;
            square.setFillColor(sf::Color::White);
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (is_clicked){
                target.draw(square);
                target.draw(text);
            }else{
                target.draw(square);
            }
        }
    
    private:
        sf::RectangleShape square;
        sf::Text text;
        sf::Font font;

        void set_up_text(void){
            if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
                std::cout << "Font not loaded" << std::endl;
            text.setFont(font);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
        }
};

#endif
