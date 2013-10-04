// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATUSBAR_H
#define STATUSBAR_H

#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>



///Generic status bar class that can be set to grow up, down, left, or, right

class StatusBar: public sf::Drawable
{
    public:
        StatusBar();
        ~StatusBar();

        void setUp(const std::string&, const sf::Vector2f&, const sf::Vector2f&, int, int, int, int, const sf::Color&, const sf::Color&, const sf::Font&, bool, bool);

        void setCurrentValue(int);
        void modifyCurrentValue(int);

        void setMaximumValue(int);
        void modifyMaximumValue(int);

        void setFillDirection(int);

        void setPosition(const sf::Vector2f&);
        void setPosition(float, float);

        void setSize(const sf::Vector2f&);
        void setSize(float, float);

        void setBackgroundColor(const sf::Color&);
        void setFillColor(const sf::Color&);

        void setValuesFont(const sf::Font&);
        void setStatusBarName(const std::string&);

        void updateBar();

        bool isEmpty();
        bool isFull();
        bool isMousedOver(sf::RenderWindow&);

        void setAlwaysVisible(bool);
        void setTextAsPercent(bool);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

    private:

        std::string statusBarName;
        sf::Text values;
        sf::FloatRect statusBar;
        sf::RectangleShape currentValueBar;
        sf::RectangleShape backgroundBar;

        sf::Color backgroundColor;
        sf::Color fillColor;

        float xPos;
        float yPos;
        float barWidth;
        float barHeight;

        //The direction that the bar fills up
        //1-Up, 2-Right, 3-Down, 4-Left
        int fillDirection;

        int minimumValue;
        int currentValue;
        int maximumValue;

        bool empty;
        bool full;
        bool mousedOver;

        bool textAlwaysShown;
        bool valuesAsPercent;
};

#endif // STATUSBAR_H
