// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAME_H
#define GAME_H

#include <string>
#include "state.h"
#include "../shared/entity.h"
#include "entitylist.h"
#include "hud.h"
#include "mainmenustate.h"
#include "../shared/map.h"

/*
This class manages the window and input for the game
*/
class PlayGameState: public State
{
    public:
        PlayGameState(GameObjects& gameObjects);
        ~PlayGameState();

        const StateAction& start(const StateArgs&);

        void handleEvents();
        void update();
        void draw();

    private:
        // Functions
        //void ReceiveData();
        void handleInput();
        void takeScreenshot();
        void handleWindowResized(GameObjects&);

        // Important objects
        Map tileMap;
        EntityList entList;
        Entity* myPlayer;
        Hud theHud; // TODO: Choose a better name?

        // Variables
        sf::Clock clock;
        sf::View gameView;
        sf::Vector2f viewDimensions;
        bool playing;
        bool paused;
        float elapsedTime;
        sf::Clock FPSTimer;
        sf::Texture playerTex, zombieTex;
        sf::Vector2i playerInput, oldPlayerInput;
};

#endif
