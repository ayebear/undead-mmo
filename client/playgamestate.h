// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAME_H
#define GAME_H

#include "gamestate.h"
#include <string>
#include "../shared/entity.h"
#include "entitylist.h"
#include "hud.h"
#include "mainmenustate.h"
#include "../shared/map.h"

// This class manages the window and input for the game

class PlayGameState: public GameState
{
    public:


        void init(GameEngine*);
        void cleanup();

        void pause();
        void resume();

        void handleEvents(GameEngine*);
        void update(GameEngine*);
        void draw(GameEngine*);

        //Return an instance of this state
        static PlayGameState* instance()
        {
            return &playState;
        }

    protected:
        PlayGameState();
    private:

        static PlayGameState playState;


        // Functions
        //void ReceiveData();
        void ProcessInput();

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
};

#endif
