// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "hud.h"

Hud::Hud()
{
}

void Hud::updateView(sf::View& mainGameView, GameObjects& objects)
{
    sf::Vector2u hudViewPos(mainGameView.getSize());
    hudView.reset(sf::Rect<float>(0, 0, hudViewPos.x, hudViewPos.y));
}

Chat& Hud::getChat()
{
    return chat;
}

void Hud::update()
{
    chat.update();
}

void Hud::setUp(GameObjects& objects)
{
    chat.setUp(sf::FloatRect(0, .77, .3, .20), objects);
}

void Hud::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    // Draw everything using the HUD view
    window.setView(hudView);

    // Draw the chat
    window.draw(chat);

    // Other HUD elements will simply be drawn here
}
