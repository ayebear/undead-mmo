#include "cursor.h"

const float Cursor::blinkRate = 0.3;
const int Cursor::endPos = -1;

Cursor::Cursor()
{
    show = false;
    rect.setSize(sf::Vector2f(2, 16));
    rect.setFillColor(sf::Color::White);
}

void Cursor::restartTimer()
{
    timer.restart();
	show = true;
}

void Cursor::update()
{
    if (timer.getElapsedTime().asSeconds() >= blinkRate)
    {
        timer.restart();
        show = !show;
    }
}

void Cursor::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    if (show)
        window.draw(rect);
}
