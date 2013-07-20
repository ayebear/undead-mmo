// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "scrollbar.h"
#include <iostream>

ScrollBar::ScrollBar()
{

    //Greyish color. semi-transparent
    scrollBar.setFillColor(noMoreText);

    isVisible = true;
}

ScrollBar::~ScrollBar()
{
    //dtor
}


//Attach the scrollBar to a view
void ScrollBar::attachScrollBar(sf::FloatRect& destRect)
{

    scrollBar.setPosition(destRect.left, destRect.top);
    scrollBar.setSize(sf::Vector2f(scrollBarWidth, destRect.height));

  /*  //Start the slider out at the bottom of the scroll bar with a height of 0.
    //Width of the slider will always be the width of the scrollBar
    slider.setPosition(scrollBar.getPosition().x, scrollBar.getPosition().y);
    slider.setSize(sf::Vector2f(scrollBarWidth, 50));*/

}

/*//Slider gets smaller when actualHeight is larger, and larger when actualHeight is smaller
void ScrollBar::adjustScrollerHeight(float viewHeight, float actualHeight)
{
    //If there is more text than what can fit inside of the view
    if(actualHeight > 0)
    {
        //Slider should be same ratio to the scrolbar as the viewHeight is to the actual height
        float sliderHeight = viewHeight * (viewHeight/actualHeight);

        slider.setSize(sf::Vector2f(scrollBarWidth, sliderHeight));
    }
    //If there is not enough text to warrant scrolling, or if there is no text
    else
        slider.setSize(sf::Vector2f(scrollBarWidth, 0));
}*/

void ScrollBar::scrollToTop(sf::View& destRect, float actualHeight, sf::Vector2f topPos)
{
    while(destRect.getCenter().y - destRect.getSize().y / 2 >= topPos.y)
    {
        scrollUp(destRect, actualHeight, topPos, actualHeight);
    }
}

void ScrollBar::scrollToBottom(sf::View& destRect, float actualHeight, sf::Vector2f bottomPos)
{
    while(destRect.getCenter().y + destRect.getSize().y / 2 <= bottomPos.y + 8)
    {
        scrollDown(destRect, actualHeight, bottomPos, actualHeight);
    }
}

float ScrollBar::scrollUp(sf::View& destRect, float actualHeight,  sf::Vector2f topPos, unsigned int distance)
{
    //Only scroll up if the slider is not at the top of the scroll bar
    if(actualHeight > 0 && distance > 0 && destRect.getCenter().y - destRect.getSize().y / 2 >= topPos.y)
    {
        unsigned int loops = 0;
        while(loops <= distance && destRect.getCenter().y - destRect.getSize().y / 2 >= topPos.y)
        {
            loops++;
            destRect.move(0, -1);
        }
        scrollBar.setPosition(scrollBar.getPosition().x, scrollBar.getPosition().y - loops);
        scrollBar.setFillColor(moreText);

        return loops;
    }

    return 0;

}
//returns distance scrolled
float ScrollBar::scrollDown(sf::View& destRect, float actualHeight, sf::Vector2f bottomPos, unsigned int distance)
{
    //Only scroll down if the bottom of the slider is not at the bottom of the scroll bar
    if(actualHeight > 0 && distance > 0 && destRect.getCenter().y + destRect.getSize().y / 2 <= bottomPos.y + 8)
    {
       unsigned int loops = 0;
       while(loops <= distance && destRect.getCenter().y + destRect.getSize().y / 2 <= bottomPos.y + 8)
       {
           loops++;
           destRect.move(0, 1);
       }

        scrollBar.setPosition(scrollBar.getPosition().x, scrollBar.getPosition().y + loops);
        if(destRect.getCenter().y + destRect.getSize().y / 2 >= bottomPos.y)
            scrollBar.setFillColor(noMoreText);

        return loops;

    }

    return 0;
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
}
