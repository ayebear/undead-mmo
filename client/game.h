// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../shared/player.h"
#include "chat.h"

// This class manages the window and input for the game

class Game
{
    public:
        Game();
        void Start();
    private:
        // Functions
        void ReceiveData(), ProcessEvents(), ProcessInput(), Update(), Display();

        // Constants
        static const int windowWidth = 800;
        static const int windowHeight = 600;

        // Variables
        sf::VideoMode vidMode;
        sf::RenderWindow window;
        sf::TcpSocket socket; // TODO: Maybe make a network management class
        Chat chat;
        bool playing;
        bool paused;
        float elapsedTime;

        Player player; // TODO: use the entity list
        sf::Texture playerTex; // TODO: Maybe make a file/resource management class
};

#endif // GAME_H
