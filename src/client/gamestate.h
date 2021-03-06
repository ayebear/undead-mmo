// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAME_H
#define GAME_H

#include <string>
#include "commonstate.h"
#include "entity.h"
#include "entitylist.h"
#include "hud.h"
#include "mainmenustate.h"
#include "tilemap.h"
#include "gamehotkeys.h"
#include "OCS/Objects/ObjectManager.hpp"
#include "OCS/Messaging/MessageHub.hpp"
#include "OCS/Systems/SystemManager.hpp"

/*
This class manages the window and input for the game
*/
class GameState: public CommonState
{
    public:
        GameState(GameObjects& gameObjects);
        ~GameState();

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
        void processEntityPacket(sf::Packet& packet);
        void processOnLogInPacket(sf::Packet& packet);
        void processMapDataPacket(sf::Packet& packet);
        void handleWindowResized();
        void loadHotkeys();

        // Important objects
        TileMap tileMap;
        EntityList entList;
        Entity* myPlayer;
        Hud theHud; // TODO: Choose a better name?
        GameHotkeys hotkeys;

        // OCS stuff
        ocs::ObjectManager objManager;
        ocs::SystemManager sysManager;
        ocs::MessageHub msgHub;

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
            Reload,
            Shoot,
            Total
        };
};

#endif
