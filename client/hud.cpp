#include "hud.h"
#include "../shared/other.h"

Hud::Hud()
{
}

void Hud::UpdateView(sf::View& mainGameView, GameObjects& objects)
{
    sf::Vector2u hudViewPos(mainGameView.getSize());
    hudView.reset(sf::Rect<float>(0, 0, hudViewPos.x, hudViewPos.y));


}

Chat& Hud::GetChat()
{
    return chat;
}

void Hud::Update()
{
    chat.Update();
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
