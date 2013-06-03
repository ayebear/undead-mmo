// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef LOGINSTATE_H
#define LOGINSTATE_H

#include "menu.h"
#include "textitemlist.h"
#include "state.h"

/*
This is for the login screen, which shows a list of servers that you can pick from.
After logging into a server, this will push a PlayGameState.
*/
class LoginState: public State
{
    public:
        LoginState(GameObjects& gameObjects);
        ~LoginState();

        const StateAction& start(const StateArgs&);

        void handleEvents();
        void update();
        void draw();

    private:
        void processChoice(int);

        Menu loginMenu;
        TextItemList textItemList;
};

#endif
