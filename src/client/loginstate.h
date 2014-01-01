// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef LOGINSTATE_H
#define LOGINSTATE_H

#include "menu.h"
#include "textitemlist.h"
#include "commonstate.h"
#include "inputbox.h"
#include "serverlistfetcher.h"

/*
This is for the login screen, which shows a list of servers that you can pick from.
After logging into a server, this will push a PlayGameState.
*/
class LoginState: public CommonState
{
    public:
        LoginState(GameObjects& gameObjects);
        ~LoginState();

        void onStart();
        const StateEvent& start(const StateArgs&);
        void takeScreenshot();

        void handleEvents();
        void update();
        void draw();

        const int fontSize = 16;

    private:
        void processChoice(int);
        void refreshServers();
        void displayServers();

        ServerListFetcher servers;
        Menu loginMenu;
        TextItemList textItemList;
        InputBox usernameBox;
        InputBox passwordBox;
        InputBox directConnectBox;
};

#endif
