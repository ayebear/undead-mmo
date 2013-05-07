#include "hud.h"
#include "../shared/other.h"

Hud::Hud()
{
    // Load font files
    if (!font.loadFromFile("data/fonts/Ubuntu-R.ttf"))
        exit(Errors::Font);
    if (!fontBold.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(Errors::Font);
    if (!fontMono.loadFromFile("data/fonts/UbuntuMono-R.ttf"))
        exit(Errors::Font);
    if (!fontMonoBold.loadFromFile("data/fonts/UbuntuMono-B.ttf"))
        exit(Errors::Font);

    chat.SetFont(&fontBold);
}

void Hud::UpdateView(sf::View& mainGameView)
{
    sf::Vector2u hudViewPos(mainGameView.getSize());
    hudView.reset(sf::Rect<float>(0, 0, hudViewPos.x, hudViewPos.y));

    chat.SetPosition(0, hudViewPos.y - 182);
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
