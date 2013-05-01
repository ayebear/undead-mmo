// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "game.h"
#include <iostream>
// This class creates a menu and processes user input
// Based on that input, different game states are run

// I will refactor my aMAZEd code for the menu, by making it completely generic, and eventually put it in here

// Maybe we should make this accept function pointers somehow? So it dereferences them and calls them depending on the menu choice?

struct MenuChoice
{
    sf::Text option;
    sf::Rect<int> rect;
    short fontSize;
    sf::Font font;
    sf::Color unselectedColor;
    sf::Color selectedColor;

};
class Menu
{
    public:

        //Game class will call the menu and pass the same screen.
        Menu(sf::RenderWindow&, sf::VideoMode);
        void Show(sf::RenderWindow&);

        int processChoice(sf::RenderWindow& screen);

        void fixRectangles(float width, float height);
    private:


        sf::Vector2f topOptionPos;

        // TODO: Use a vector or some container of MenuChoice objects instead.
        // Could be done like this: "vector<MenuChoice> options;"
        // Then all of the code could just loop through that and use math to position it.
        // And we could more easily add new menu options, and maybe even allow adding stuff from outside the class,
        // so that it is more generic.
        MenuChoice play;
        MenuChoice options;
        MenuChoice quit;
        string tmpText;

        short fontSize;
        sf::Font font;
        sf::Color unselectedColor;
        sf::Color selectedColor;

        sf::RenderWindow window;
        sf::VideoMode vidMode;
        sf::View view;

        sf::Texture background;
        sf::Sprite backgroundImage;

        int selection;
};

#endif // MENU_H
