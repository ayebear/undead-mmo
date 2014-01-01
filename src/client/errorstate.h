// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef ERRORSTATE_H
#define ERRORSTATE_H

#include "menu.h"
#include "commonstate.h"

class ErrorState: public CommonState
{
    public:
        ErrorState(GameObjects& gameObjects);
        ~ErrorState();

        const StateEvent& start(const StateArgs&);

        void processArgs(const StateArgs&);

        void handleEvents();
        void update();
        void draw();

    private:
        void processChoice(int);

        Menu errorMenu;
        sf::Text headingText;
        sf::Text errorText;
        sf::Font errorFont;
};

#endif
