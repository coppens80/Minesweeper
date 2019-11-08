#ifndef SQUARE
#define SQUARE

#include <iostream>
#include <sstream>

class GameTile : public sf::Drawable {
    public:    
        float box_size = 25.f;
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

        bool click(const sf::Event event) {
            if(tile.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)){
                is_clicked = true;
                tile.setFillColor(sf::Color::White);
                return true;
            }
            return false;
        }
        
        void flag(const sf::Event event, int &num_flags) {
            if(tile.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y) && !is_clicked){
                if (!flagged && num_flags > 0){
                    flagged = true;
                    tile.setFillColor(sf::Color::Blue);
                    --num_flags;
                }else if (flagged){
                    flagged = false;
                    tile.setFillColor(sf::Color(84, 84, 84, 255));
                    ++num_flags;
                }
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
            tile.setSize(sf::Vector2f(box_size, box_size));
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
