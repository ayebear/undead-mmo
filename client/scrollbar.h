// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include <SFML/Graphics.hpp>

/**
    Generic scrollbar class that can be attached to any view
    Contains a slider with a dynamic height that changes based on how much scrolling can be done.
*/
class ScrollBar: public sf::Drawable
{
    public:
        ScrollBar();
        ~ScrollBar();

        ///Attach the scrollbar to a view
        void attachScrollBar(sf::FloatRect&);

        ///Slider gets smaller when actualHeight is larger, and larger when actualHeight is smaller
        void adjustScrollerHeight(float viewHeight, float actualHeight);

        float scrollUp(sf::View&, float, sf::Vector2f, unsigned int);
        float scrollDown(sf::View&, float, sf::Vector2f, unsigned int);

        void setVisible(bool);
        bool getVisible();

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        const int scrollBarWidth = 10;
    private:

        ///The scroll bar
        sf::RectangleShape scrollBar;

        ///Colors that show whether or not there is text below
        const sf::Color moreText = sf::Color(200, 0, 0, 75);
        const sf::Color noMoreText = sf::Color(10, 10, 10, 75);
        bool isVisible;

};

#endif // SCROLLBAR_H
