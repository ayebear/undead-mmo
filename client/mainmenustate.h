// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "state.h"
#include "menu.h"

/*
This class creates a menu and processes user input
Based on that input, a different action is returned from this state to the state manager
*/
class MainMenuState: public State
{
    public:
        MainMenuState(GameObjects& gameObjects);
        ~MainMenuState();

        const StateAction& start(const StateArgs&);

        void handleEvents();
        void update();
        void draw();

    private:
        void processChoice(int);

        Menu mainMenu;
};

#endif
