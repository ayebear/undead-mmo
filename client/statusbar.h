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

        void setUp(std::string&, sf::Vector2f&, sf::Vector2f&, int, int, int, int, sf::Color&, sf::Color&, sf::Font&, bool, bool);

        void setCurrentValue(int);
        void modifyCurrentValue(int);

        void setMaximumValue(int);
        void modifyMaximumValue(int);

        void setFillDirection(int);

        void setPosition(sf::Vector2f&);
        void setPosition(float, float);

        void setSize(sf::Vector2f&);
        void setSize(float, float);

        void setBackgroundColor(sf::Color&);
        void setFillColor(sf::Color&);

        void setValuesFont(sf::Font&);
        void setStatusBarName(std::string&);

        void updateBar();

        bool isEmpty();
        bool isFull();
        bool isMousedOver(sf::RenderWindow&);

        bool setAlwaysVisible(bool);
        bool setTextAsPercent(bool);

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
