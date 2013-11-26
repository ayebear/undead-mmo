// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAME_H
#define GAME_H

#include <string>
#include "state.h"
#include "entity.h"
#include "entitylist.h"
#include "hud.h"
#include "mainmenustate.h"
#include "map.h"
#include "gamehotkeys.h"

/*
This class manages the window and input for the game
*/
class PlayGameState: public State
{
    public:
        PlayGameState(GameObjects& gameObjects);
        ~PlayGameState();

        void onStart();
        const StateEvent& start(const StateArgs&);

        void onPush();
        void onPop();

        void handleEvents();
        void update();
        void draw();

    private:
        // Functions
        void updateGameView();
        void handleKeyPressed(sf::Keyboard::Key);
        void handleKeyReleased(sf::Keyboard::Key);
        void handleInput();
        void handleMouseInput();
        void sendAngleInputPacket();
        void processEntityPackets();
        void processOnLogInPackets();
        void processMapDataPackets();
        void handleWindowResized();
        void loadHotkeys();

        // Important objects
        Map tileMap;
        EntityList entList;
        Entity* myPlayer;
        Hud theHud; // TODO: Choose a better name?
        GameHotkeys hotkeys;

        // Variables
        sf::Clock clock;
        sf::View gameView;
        sf::Vector2f viewDimensions;
        bool playing;
        bool hasFocus;
        float elapsedTime;
        sf::Clock FPSTimer;

        // This is a lot of crap just for the player movement, maybe it can all be moved into the entity?
        sf::Vector2i playerInput, oldPlayerInput;
        bool playerIsMoving;
        EID myPlayerId;
        float currentAngle;
        float lastSentAngle;
        sf::Clock angleTimer;
        bool mouseMoved;

        // The inventory should be able to handle this
        sf::Clock inventoryTimer;
        bool inventoryKeyReleased;

        // Currently just keyboard events, until we have a fully generic event handler
        enum Event
        {
            MoveUp = 0,
            MoveDown,
            MoveLeft,
            MoveRight,
            MoveUpAlt,
            MoveDownAlt,
            MoveLeftAlt,
            MoveRightAlt,
            ToggleInventory,
            ToggleInventoryAlt,
            ShowChat,
            HideChat,
            TakeScreenshot,
            AddSlot,
            RemoveSlot,
            Total
        };
};

#endif
