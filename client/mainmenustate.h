// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "gamestate.h"
#include "menu.h"
#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

// This class creates a menu and processes user input
// Based on that input, different game states are run

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


    protected:
        MainMenuState();
        ~MainMenuState();

    private:

        static MainMenuState menuState;

        Menu mainMenu;

};

#endif // MENU_H
