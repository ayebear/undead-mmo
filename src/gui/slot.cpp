// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "slot.h"
#include <sstream>
#include <string>

Slot::Slot()
{

    backgroundEmpty = sf::Color(10, 10, 10, 75);
    backgroundNonEmpty = sf::Color(128, 128, 128, 150);

    activeColors[inActive] = sf::Color(10,10,10,150);
    activeColors[highlighted] = sf::Color(255, 255, 255, 200);
    activeColors[activeLeft] = sf::Color(200, 0, 0, 200);
    activeColors[activeRight] = sf::Color(0, 0, 200, 200);

    slotBackground.setFillColor(backgroundEmpty);
    slotBackground.setOutlineColor(activeColors[inActive]);
    slotBackground.setOutlineThickness(1);

    isEmpty = true;
    isHighlighted = false;
    textVisible = true;
    fontSet = false;
    active = inActive;
}

Slot::~Slot()
{
    //dtor
}


void Slot::setBackgroundColors(const sf::Color& emptyBackgr, const sf::Color& nonEmptyBackgr)
{
    backgroundEmpty = emptyBackgr;
    backgroundNonEmpty = nonEmptyBackgr;
}

void Slot::setEmptyBackgroundColor(const sf::Color& emptyBackgr)
{
    backgroundEmpty = emptyBackgr;
}

void Slot::setNonEmptyBackgroundColor(const sf::Color& nonEmptyBackgr)
{
    backgroundNonEmpty = nonEmptyBackgr;
}

void Slot::setAllOutlineColors(const sf::Color& defaultColor, const sf::Color& highlightColor, const sf::Color& left, const sf::Color& right)
{
    activeColors[inActive] = defaultColor;
    activeColors[highlighted] = highlightColor;
    activeColors[activeLeft] = left;
    activeColors[activeRight] = right;
}

void Slot::setOutlineColor(int activeType, sf::Color& outlineColor)
{
    activeColors[activeType] = outlineColor;
}

void Slot::setActive(int activeType)
{
    if (activeType < totalActiveTypes && activeType >= 0)
        active = activeType;
    updateOutlineColor();
}

void Slot::toggleActive(int activeType)
{
    if (activeType == active)
        setInactive();
    else
        setActive(activeType);
}

void Slot::setInactive()
{
    active = inActive;
    updateOutlineColor();
}

void Slot::setPosition(const sf::Vector2f& pos)
{
    slot.setPosition(pos);
    slotBackground.setPosition(pos);
    slotRect.left = pos.x;
    slotRect.top = pos.y;
    text.setPosition(pos.x + 2, pos.y + slotRect.height - 20);
}

void Slot::setSize(const sf::Vector2f& size)
{
    slot.setSize(size);
    slotRect.width = size.x;
    slotRect.height = size.y;
    slotBackground.setSize(size);
}

void Slot::addItem(const sf::Texture& tex)
{
    slotBackground.setFillColor(backgroundNonEmpty);
    slot.setTexture(&tex);
    isEmpty = false;
}

void Slot::removeItem()
{
    slotBackground.setFillColor(backgroundEmpty);
    isEmpty = true;
}

sf::Vector2f Slot::getPosition() const
{
    return slotBackground.getPosition();
}

void Slot::setFont(const sf::Font& font)
{
    text.setFont(font);
    text.setCharacterSize(16);
    fontSet = true;
}

sf::Vector2f Slot::getSize() const
{
    return slotBackground.getSize();
}

bool Slot::handleMouseMoved(sf::Event event)
{
    return updateHighlighted(event.mouseMove.x, event.mouseMove.y);
}

void Slot::setText(const std::string& str)
{
    if (fontSet)
        text.setString(str);
}

void Slot::setText(int val)
{
    if (fontSet)
        text.setString(std::to_string(val));
}

void Slot::showText(bool show)
{
    textVisible = show;
}

bool Slot::handleMouseClicked(sf::Event event)
{
    return updateHighlighted(event.mouseButton.x, event.mouseButton.y);
}

void Slot::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(slotBackground);

    if (!isEmpty)
        window.draw(slot);

    if (textVisible)
        window.draw(text);
}

bool Slot::updateHighlighted(int x, int y)
{
    isHighlighted = slotRect.contains(x, y);
    updateOutlineColor();
    return isHighlighted;
}

void Slot::updateOutlineColor()
{
    int activeState = (isHighlighted ? highlighted : active);
    slotBackground.setOutlineColor(activeColors[activeState]);
}
