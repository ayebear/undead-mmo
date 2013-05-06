// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "../shared/entity.h"
#include "entitylist.h"
#include "chat.h"
#include "menu.h"
#include "../shared/map.h"

// This class manages the window and input for the game

class Game
{
    public:
        Game();
        void Start();
    private:
        // Functions
        void ReceiveData();
        void ProcessEvents();
        void ProcessInput();
        void Update();
        void Display();

        // Constants
        static const int windowWidth = 800;
        static const int windowHeight = 600;

        // Variables
        sf::VideoMode vidMode;
        sf::RenderWindow window;

        sf::View gameView;

        sf::View chatView;


        Map tileMap;

        sf::TcpSocket socket; // TODO: Make a network management class
        Chat chat;
        bool playing;
        bool paused;
        float elapsedTime;
        sf::Clock FPSTimer;
        sf::Font font, fontBold, fontMono, fontMonoBold;
        sf::Texture playerTex, zombieTex;

        EntityList entList;
        Entity* myPlayer;
};

#endif
