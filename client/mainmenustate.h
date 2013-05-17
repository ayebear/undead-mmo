// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include "gamestate.h"
#include <iostream>
#include <vector>
using namespace std;


// This class creates a menu and processes user input
// Based on that input, different game states are run


struct MenuChoice
{
    sf::Text option;
    sf::Rect<int> rect;
    sf::Color unselectedColor;
    sf::Color selectedColor;
};

class MainMenuState: public GameState
{
    public:

        //Game class will call the menu and pass the same screen.

        void init(GameEngine*);
        void cleanup();

        void pause();
        void resume();

        void handleEvents(GameEngine*);
        void update(GameEngine*);
        void draw(GameEngine*);

        //Return an instance of this state
        static MainMenuState* instance()
        {
            return &menuState;
        }


        void fixRectangles(float width, float height);

    protected:
        MainMenuState();
        ~MainMenuState();
    private:

        static MainMenuState menuState;

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
        bool selectionMade;
};

#endif // MENU_H
