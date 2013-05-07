// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "game.h"
#include <iostream>
#include <vector>

// This class creates a menu and processes user input
// Based on that input, different game states are run

// I will refactor my aMAZEd code for the menu, by making it completely generic, and eventually put it in here
// Nevermind, this menu is fine.

struct MenuChoice
{
    sf::Text option;
    sf::Rect<int> rect;
    sf::Color unselectedColor;
    sf::Color selectedColor;
};

class Menu
{
    public:

        //Game class will call the menu and pass the same screen.
        Menu(sf::RenderWindow&, sf::VideoMode);
        ~Menu();

        void Show(sf::RenderWindow&);

        int processChoice(sf::RenderWindow& screen);

        void fixRectangles(float width, float height);
    private:

        void addMenuItem(string itemName);

        sf::Vector2f topOptionPos;

        vector<MenuChoice*> menuOptions;
        string tmpText;

        short fontSize;
        sf::Font font;
        sf::Color unselectedColor;
        sf::Color selectedColor;

        sf::RenderWindow window;
        sf::VideoMode vidMode;
        sf::View view;

        sf::Texture bgTexture;
        sf::Sprite bgSprite;

        int selection;
};

#endif // MENU_H
