#include "hud.h"

Hud::Hud()
{
}

void Hud::UpdateView(sf::View& mainGameView, GameObjects& objects)
{
    sf::Vector2u hudViewPos(mainGameView.getSize());
    hudView.reset(sf::Rect<float>(0, 0, hudViewPos.x, hudViewPos.y));

    //MessageBox is 512.
    chat.setUp(0, hudViewPos.y / 1.75, mainGameView.getSize().x / 3, mainGameView.getSize().y / 3, objects);
}

Chat& Hud::GetChat()
{
    return chat;
}

void Hud::Update()
{
    chat.Update();
}


void Hud::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    // Draw everything using the HUD view
    window.setView(hudView);

    // Draw the chat
    window.draw(chat);

    // Other HUD elements will simply be drawn here
}
