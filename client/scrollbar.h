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

        void scrollUp(sf::View&, sf::Vector2f, unsigned int);
        void scrollDown(sf::View&, sf::Vector2f, unsigned int);

        void setVisible(bool);
        bool getVisible();

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        const int scrollBarWidth = 10;
    private:

        ///The slider that is connected to the scroll bar and rhe actual scroll bar
        sf::RectangleShape slider;
        sf::RectangleShape scrollBar;

        bool isVisible;

};

#endif // SCROLLBAR_H
