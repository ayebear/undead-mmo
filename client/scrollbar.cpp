#include "scrollbar.h"
#include <iostream>
ScrollBar::ScrollBar()
{
    //Almost white. More opaque
    slider.setFillColor(sf::Color(0, 0, 0, 150));

    //Greyish color. semi-transparent
    scrollBar.setFillColor(sf::Color(200, 200, 200, 50));

    isVisible = true;
}

ScrollBar::~ScrollBar()
{
    //dtor
}


///Attach the scrollBar to a view,
void ScrollBar::attachScrollBar(sf::FloatRect& destRect)
{

    scrollBar.setPosition(destRect.left, destRect.top);
    scrollBar.setSize(sf::Vector2f(scrollBarWidth, destRect.height));

    //Start the slider out at the bottom of the scroll bar with a height of 0.
    //Width of the slider will always be the width of the scrollBar
    slider.setPosition(scrollBar.getPosition().x, scrollBar.getPosition().y);
    slider.setSize(sf::Vector2f(scrollBarWidth, 50));

}

//Slider gets smaller when actualHeight is larger, and larger when actualHeight is smaller
void ScrollBar::adjustScrollerHeight(float viewHeight, float actualHeight)
{
    //If there is more text than what can fit inside of the view
    if(actualHeight > 0)
    {
        //Slider should be same ratio to the scrolbar as the viewHeight is to the actual height
        float sliderHeight = scrollBar.getSize().y * (viewHeight/actualHeight);

        slider.setSize(sf::Vector2f(scrollBarWidth, sliderHeight));
    }
    //If there is not enough text to warrant scrolling, or if there is no text
    else
        slider.setSize(sf::Vector2f(scrollBarWidth, 0));
}

void ScrollBar::scrollUp(sf::View& destRect, sf::Vector2f topPos, unsigned int distance)
{

    //Only scroll up if the slider is not at the top of the scroll bar
    if(destRect.getCenter().y - destRect.getSize().y / 2 >= topPos.y)
    {
        unsigned int loops = 0;
        while(loops < distance && destRect.getCenter().y - destRect.getSize().y / 2 >= topPos.y)
        {
            loops++;
            destRect.move(0, -1);
            slider.setPosition(slider.getPosition().x, slider.getPosition().y - 1);
        }
    }

}
void ScrollBar::scrollDown(sf::View& destRect, sf::Vector2f bottomPos, unsigned int distance)
{
    //Only scroll down if the bottom of the slider is not at the bottom of the scroll bar
    if(destRect.getCenter().y + destRect.getSize().y / 2 <= bottomPos.y + 3)
    {
       unsigned int loops = 0;
       while(loops <= distance && destRect.getCenter().y + destRect.getSize().y / 2 <= bottomPos.y + 3)
       {
           loops++;
           destRect.move(0, 1);
           slider.setPosition(slider.getPosition().x, slider.getPosition().y + 1);

       }
    }
}

void ScrollBar::setVisible(bool visible)
{
    isVisible = visible;
}
bool ScrollBar::getVisible()
{
    return isVisible;
}

void ScrollBar::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(scrollBar);
    window.draw(slider);
}
