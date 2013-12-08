// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef HUD_H
#define HUD_H

#include "SFML/Graphics.hpp"
#include "inventorygui.h"
#include "chat.h"
#include "gameobjects.h"
#include "statusbar.h"

// This class contains all of the user interface elements for the game.
class Hud: public sf::Drawable
{
    public:
        Hud();
        void updateView(sf::View&);
        void update();
        void setUp(GameObjects&);
        void handleMouseMoved(sf::Event&, sf::RenderWindow&);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        Chat chat;
        StatusBar healthBar;
        StatusBar infectionBar;
        InventoryGUI inventory;
        // Add other HUD elements later, like mini-map/health/infection/etc.

    private:
        sf::RenderWindow* window;
        sf::View hudView;
};

#endif
