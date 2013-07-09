#ifndef HUD_H
#define HUD_H

#include "SFML/Graphics.hpp"
#include "chat.h"
#include "gameobjects.h"

// This class contains all of the user interface elements for the game.
class Hud: public sf::Drawable
{
    public:
        Hud();
        void updateView(sf::View&, GameObjects&);
        Chat& getChat();
        void update();
        void setUp(GameObjects&);

        void setWindow(sf::RenderWindow&);
        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        // TODO: I guess we could use getChat() but it is so annoying... And then you still have full access from the outside anyway...
        Chat chat;
        // Add other HUD elements later, like mini-map/health/infection/etc.

    private:
        sf::RenderWindow* window;
        sf::View hudView;
};

#endif
