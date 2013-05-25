#ifndef LOGINSTATE_H
#define LOGINSTATE_H

#include "gamestate.h"
#include "menu.h"
#include "textitemlist.h"


class LoginState : public GameState
{
    public:
        LoginState();
        ~LoginState();

        void init(GameEngine*);
        void cleanup();

        void pause();
        void resume();

        void handleEvents(GameEngine*);
        void update(GameEngine*);
        void draw(GameEngine*);

        //Return an instance of this state
        static LoginState* instance()
        {
            return &loginState;
        }

    private:
        static LoginState loginState;

        Menu loginMenu;
        TextItemList textItemList;
};

#endif // LOGINSTATE_H
