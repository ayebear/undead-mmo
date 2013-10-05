// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "hud.h"

Hud::Hud()
{
}

void Hud::updateView(sf::View& mainGameView)
{
    sf::Vector2u hudViewPos(mainGameView.getSize());
    hudView.reset(sf::Rect<float>(0, 0, hudViewPos.x, hudViewPos.y));
}

void Hud::update()
{
    chat.update();
}

void Hud::setUp(GameObjects& objects)
{
    sf::Color statusBarBackgroundCol(190, 190, 190, 75);

    //StatusBar placement and sizing is temporary
    sf::Vector2f healthBarPos(objects.window.getSize().x / 1.2, objects.window.getSize().y / 1.053);
    sf::Vector2f healthBarSize(objects.window.getSize().x / 8, 15);
    sf::Color healthBarFillColor(250, 20, 20, 200);
    std::string healthBarName("Health");

    sf::Vector2f infectionBarPos(objects.window.getSize().x / 1.2, objects.window.getSize().y / 1.023);
    sf::Vector2f infectionBarSize(objects.window.getSize().x / 8, 15);
    sf::Color infectionBarFillColor(150, 0, 200, 200);
    std::string infectionBarName("Infection");

    //Testing vertical growth
    //sf::Vector2f healthBarPos(objects.window.getSize().x / 1.2, objects.window.getSize().y / 2);
    //sf::Vector2f healthBarSize(15, objects.window.getSize().y / 8);

    chat.setUp(sf::FloatRect(0, .76, .3, .20), objects);
    healthBar.setUp(healthBarName, healthBarPos, healthBarSize, 0, 1000, 1000, 1, statusBarBackgroundCol, healthBarFillColor, objects.fontBold, false, false);
    infectionBar.setUp(infectionBarName, infectionBarPos, infectionBarSize, 0, 50, 100, 2, statusBarBackgroundCol, infectionBarFillColor, objects.fontBold, false, true);

    inventory.setUp(1, sf::FloatRect(.7, .3, .3, .6), objects.fontBold, objects.window);
}

void Hud::handleMouseMoved(sf::Event& event, sf::RenderWindow& window)
{
    healthBar.isMousedOver(window);
    infectionBar.isMousedOver(window);
}

void Hud::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    // Draw everything using the HUD view
    window.setView(hudView);

    // Draw the chat
    window.draw(chat);
    // Other HUD elements will simply be drawn here

    window.draw(healthBar);
    window.draw(infectionBar);
    window.draw(inventory);
}
