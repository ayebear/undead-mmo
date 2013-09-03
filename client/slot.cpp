#include "slot.h"

Slot::Slot()
{

    backgroundEmpty = sf::Color(10, 10, 10, 75);
    backgroundNonEmpty = sf::Color(200, 200, 200, 150);

    activeColors[inActive] = sf::Color(10,10,10,150);
    activeColors[highlighted] = sf::Color(255, 255, 255, 200);
    activeColors[activeLeft] = sf::Color(200, 0, 0, 200);
    activeColors[activeRight] = sf::Color(0, 0, 200, 200);

    slotBackground.setFillColor(backgroundEmpty);

    slotBackground.setOutlineColor(activeColors[inActive]);

    slotBackground.setOutlineThickness(1);

    isEmpty = true;
    active = inActive;
}

Slot::~Slot()
{
    //dtor
}


void Slot::setBackgroundColors(sf::Color& emptyBackgr, sf::Color& nonEmptyBackgr)
{
    backgroundEmpty = emptyBackgr;
    backgroundNonEmpty = nonEmptyBackgr;
}

void Slot::setEmptyBackgroundColor(sf::Color& emptyBackgr)
{
    backgroundEmpty = emptyBackgr;
}

void Slot::setNonEmptyBackgroundColor(sf::Color& nonEmptyBackgr)
{
    backgroundNonEmpty = nonEmptyBackgr;
}

void Slot::setAllOutlineColors(sf::Color& defaultColor, sf::Color& highlightColor, sf::Color& left, sf::Color& right)
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
    if(activeType < totalActiveTypes && activeType >= 0)
        active = activeType;

    slot.setOutlineColor(activeColors[active]);

}
void Slot::toggleActive(int activeType)
{
    if(activeType == active)
    {
        setInactive();
    }
    else
        setActive(activeType);
}


void Slot::setInactive()
{
    active = inActive;

    slot.setOutlineColor(activeColors[active]);
}


void Slot::setPosition(sf::Vector2f pos)
{
    slot.setPosition(pos);
    slotBackground.setPosition(pos);
    slotRect.left = pos.x;
    slotRect.top = pos.y;
}

void Slot::setSize(sf::Vector2f size)
{
    slot.setSize(size);
    slotRect.width = size.x;
    slotRect.height = size.y;
    slotBackground.setSize(size);
}

void Slot::addItem(sf::Texture& tex)
{
    slotBackground.setFillColor(backgroundNonEmpty);
    slot.setTexture(&tex);
    isEmpty = false;
}

void Slot::removeItem()
{
    slotBackground.setFillColor(backgroundEmpty);
    slot.setFillColor(sf::Color::Transparent);
    isEmpty = true;
}

sf::Vector2f Slot::getPosition() const
{
    return slot.getPosition();
}

sf::Vector2f Slot::getSize() const
{
    return slot.getSize();
}

bool Slot::handleMouseMoved(sf::Event& event, sf::RenderWindow& window)
{
    bool hoveredOver = false;
    if(slotRect.contains(event.mouseMove.x, event.mouseMove.y))
    {
        hoveredOver = true;
        slotBackground.setOutlineColor(activeColors[highlighted]);
    }
    else
        slotBackground.setOutlineColor(activeColors[active]);

    return hoveredOver;

}

bool Slot::handleMouseClicked(sf::Event& event, sf::RenderWindow& window)
{
    bool clicked = false;

    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getView());

    if(slotRect.contains(mousePos))
        clicked = true;

    return clicked;

}

void Slot::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(slotBackground);

    if(!isEmpty)
        window.draw(slot);
}
