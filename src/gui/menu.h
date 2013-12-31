// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

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
    sf::RectangleShape buttonBackground;
    sf::Color unselectedColor;
    sf::Color selectedColor;

    short fontSize;
    sf::Font font;
};

class Menu: public sf::Drawable
{
    public:

        Menu();
        ~Menu();

        //Background file, font file, font size, topButtonPos, the window, and the desired view
        void setUpMenu(const std::string&, sf::Color, short, sf::Vector2f, sf::RenderWindow*, sf::Font*);

        void updateMenu();
        void clearButtons();

        void setBackground(const std::string&);
        void setFont(const std::string&);
        void setFontSize(short);

        void setTopButtonLocation(sf::Vector2f);
        void setButtonColors(sf::Color);

        void addMenuButton(const std::string&);
        void addMenuButton(const std::string&, sf::Vector2f);
        void createLabel(const std::string&, sf::Vector2f);

        //Returns 1 if first button is pressed, 2 if second button is pressed, etc.
        //Returns -1 if window is closed.
        int handleEvents();

        void handleMouseMovement(sf::Event&);
        int handleMouseReleased(sf::Event&);
        void handleMousePressed(sf::Event&);
        int handleKeyPressed(sf::Event&);
        void handleResize(sf::Event&);

        virtual void draw(sf::RenderTarget&, sf::RenderStates) const;


    private:

        sf::RenderWindow* window;

        //Utility Functions
        void fixRectangles(float, float);

        //Formatting variables
        sf::Color buttonUnselectedColor;
        sf::Color buttonSelectedColor;
        sf::Color buttonPressedColor;
        sf::Color buttonStandardColor;
        sf::Color buttonHighlightedColor;
        sf::Font* buttonFont;
        short buttonFontSize;
        sf::Vector2f topButtonPos;

        //Background
        sf::Texture bgTexture;
        sf::Sprite bgSprite;
        sf::Vector2u bgImageSize;

        //Used to keep track of the size of the window before resizing
        sf::Vector2u windowSize;
        sf::View menuView;

        //Used to reorientate the buttons on resize
        float buttonWidthFactor;
        float buttonHeightFactor;

        int selection;
        bool selectionMade;

        const int buttonPadding = 20;

        //Array of buttons that are on the screen
        std::vector<MenuButton*> menuOptions;
        std::vector<sf::Text> labels;
};

#endif // MENU_H
