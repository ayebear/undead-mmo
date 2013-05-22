#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

/*
    Menu class that can be used as a component for any type of menu screen.
*/

//Clickable buttons
struct MenuButton
{
    //The name of the button
    sf::Text buttonName;
    sf::Rect<int> rect;
    sf::Color unselectedColor;
    sf::Color selectedColor;

    short fontSize;
    sf::Font font;
};

class Menu: public sf::Drawable
{
    public:

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;

        Menu();
        ~Menu();

        //Background file, font file, font size, topButtonPos, the window, and the desired view
        void setUpMenu(std::string&, std::string&, short, sf::Vector2f, sf::RenderWindow*);

        //Returns 1 if first button is pressed, 2 if second button is pressed, etc.
        //Returns -1 if window is closed.
        int handleEvents(sf::RenderWindow&);
        void updateMenu();
        void clearButtons();

        void setBackground(std::string&);
        void setFont(std::string&);
        void setFontSize(short);

        void setTopButtonLocation(sf::Vector2f);
        void setButtonColors(sf::Color, sf::Color);

        void addMenuButton(std::string);

    private:

        //Utility Functions
        void fixRectangles(float, float);


        //Formatting variables
        sf::Color buttonUnselectedColor;
        sf::Color buttonSelectedColor;
        sf::Font buttonFont;
        short buttonFontSize;
        sf::Vector2f topButtonPos;

        //Background
        sf::Texture bgTexture;
        sf::Sprite bgSprite;
        sf::Vector2u bgImageSize;

        //Used to keep track of the size of the window before resizing
        sf::Vector2f windowSize;

        //Used to reorientate the buttons on resize
        float buttonWidthFactor;
        float buttonHeightFactor;

        int selection;
        bool selectionMade;

        //Array of buttons that are on the screen
        std::vector<MenuButton*> menuOptions;
};

#endif // MENU_H
