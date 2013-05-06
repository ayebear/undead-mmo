#include "hud.h"


void Hud::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
     //Draw the chat in its own view
    window.setView(chatView);

    // Draws the chat
    window.draw(chat);

    window.setView(gameView);
}

Chat Hud::getChat()
{
    return chat;
}
void Hud::fixViews(sf::View mainGameView)
{
    gameView.setSize(mainGameView.getSize());
    gameView.setCenter(mainGameView.getCenter());

    sf::Vector2u chatViewPos(gameView.getSize());
    chatView.reset(sf::Rect<float>(0, 0, chatViewPos.x, chatViewPos.y));
}

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
