#ifndef ERRORSTATE_H
#define ERRORSTATE_H

#include "menu.h"
#include "state.h"

class ErrorState: public State
{
    public:
        ErrorState(GameObjects& gameObjects);
        ~ErrorState();

        const StateAction& start(const StateArgs&);

        void processArgs(const StateArgs& args);

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
