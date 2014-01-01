// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef MESSAGESTATE_H
#define MESSAGESTATE_H

#include "menu.h"
#include "commonstate.h"

class MessageState: public CommonState
{
    public:
        MessageState(GameObjects& gameObjects);
        ~MessageState();

        const StateEvent& start(const StateArgs&);

        void processArgs(const StateArgs&);

        void handleEvents();
        void update();
        void draw();

    private:
        void processChoice(int);

        Menu messageMenu;
        sf::Text messageText;
        sf::Font messageFont;
};

#endif
