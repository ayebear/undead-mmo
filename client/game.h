#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../shared/player.h"

// This class manages the window and input for the game

const std::string version = "Project: Brains v0.0.0.5 Dev";

class Game
{
    public:
        Game();
        void Start();
    private:
        // Functions
        void GetInput(), ProcessEvents(), ProcessInput(), Update(), Display();

        // Constants
        static const int windowWidth = 1024;
        static const int windowHeight = 768;

        // Variables
        sf::VideoMode vidMode;
        sf::RenderWindow window;
        bool playing;
        float elapsedTime;

        Player player; // TODO: use the entity list
        sf::Texture playerTex; // TODO: make a file loading/resource manager class or something
};

#endif // GAME_H
