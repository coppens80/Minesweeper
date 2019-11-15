//SRC: J.Coppens 2019

#ifndef SQUARE
#define SQUARE

#include <iostream>
#include <sstream>

class GameTile : public sf::Drawable {
    public:    
        float size = 25.f;
        int val = 0;
        bool is_mine = false;
        bool flagged = false;
        bool is_clicked = false;
    
    public:
        GameTile(void){
            setup_tile();
            setup_text();
        }
    
        void set_position(float x, float y){
            tile.setPosition(x, y);
            text.setPosition(x+6, y);
        }
        
        void set_value(int i){
            val = i;
            if(val == 0)
                text.setString(" ");
            else
                text.setString(std::to_string(val).c_str());
        }

        void create_mine(void){
            val = -1;
            is_mine = true;
            text.setString('x');
            text.setFillColor(sf::Color::Black);
        }

        void click(void) {
            is_clicked = true;
            tile.setFillColor(sf::Color::White);
        }
        
        void flag(int &num_flags) {
            if (!flagged){
                flagged = true;
                tile.setFillColor(sf::Color::Blue);
                --num_flags;
            }else{
                flagged = false;
                tile.setFillColor(sf::Color(84, 84, 84, 255));
                ++num_flags;
            }
        }

        void reveal(void){
            is_clicked = true;
            tile.setFillColor(sf::Color::White);
            if (is_mine)
                tile.setFillColor(sf::Color::Red);
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (is_clicked){
                target.draw(tile);
                target.draw(text);
            }else{
                target.draw(tile);
            }
        }
    
    private:
        sf::RectangleShape tile;
        sf::Text text;
        sf::Font font;

        void setup_tile(void){
            tile.setSize(sf::Vector2f(size, size));
            tile.setFillColor(sf::Color(84, 84, 84, 255));
            tile.setOutlineThickness(2.f);
            tile.setOutlineColor(sf::Color::Black);
        }
            
        void setup_text(void){
            if (!font.loadFromFile("../dep/Fonts/Arial.ttf"))
                std::cout << "Font not loaded" << std::endl;
            text.setFont(font);
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
        }
};

#endif
