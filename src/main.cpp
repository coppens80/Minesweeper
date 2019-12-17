//SRC: J.Coppens 2019

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <sstream>
#include "minesweeper.h"
#include "AI.h"
#include <stdlib.h>

int main(int argc, char** argv){

    std::string difficulty;
    if (argc > 1)
        if (std::string(argv[1]) == "Easy" || std::string(argv[1]) == "Medium" || std::string(argv[1]) == "Hard")
            difficulty = argv[1];
        else{
            std::cout << "Error: Invalid game mode. Available modes: Easy, Medium, Hard" << std::endl;
            exit(1);
        }
    else
        difficulty = "Easy";

    Minesweeper game(difficulty);
    MinesweeperAI solver(&game);
    auto [width, height] = game.window_size;
    
    sf::RenderWindow window(sf::VideoMode(width, height), "My Shitty Minesweeper");
    window.setKeyRepeatEnabled(false);
    
    sf::Event event;
    sf::Clock timer;
    int n = 0;

    while (window.isOpen()) {
         
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();
            
            if (event.type == sf::Event::MouseButtonPressed && !game.game_over){
                /* std::cout << event.mouseButton.y << " " << event.mouseButton.x << std::endl; */
                if (event.mouseButton.button == sf::Mouse::Left)
                    game.left_click(event.mouseButton.x, event.mouseButton.y);
                if (event.mouseButton.button == sf::Mouse::Right)
                    game.right_click(event.mouseButton.x, event.mouseButton.y);
            }

            if (event.type == sf::Event::KeyPressed){
                if (event.key.code == sf::Keyboard::N){
                    solver.read_board();
                    solver.compute();
                }
                /* if (event.key.code == sf::Keyboard::M){ */
                /*     solver.read_board(); */
                /*     solver.tank_solver(); */
                /* } */
                if (event.key.code == sf::Keyboard::R){
                    printf("==================\n");
                    game.reset();
                    /* std::cout << "Average frame time: " << timer.getElapsedTime().asSeconds() * 1000 / n << std::endl; */
                    timer.restart();
                }
            }
        
        }
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && 
            sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            window.close();

        game.draw_board(window);
        n++;
    }

    std::cout << "Average frame time: " << timer.getElapsedTime().asSeconds() * 1000 / n << std::endl;

    return 0;
}

