// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include "gameobjects.h"
#include "state.h"

/*
This class handles the allocation/deallocation/starting/changing of all types of game states.
*/
class StateManager
{
    public:
        StateManager(std::string, int, int); // Windowed
        StateManager(std::string); // Fullscreen
        ~StateManager();

        void startLoop(const StateAction&);
        const StateAction& handleAction(const StateAction&);

        void push(int);
        void pop();

        void fail();

    private:
        std::vector<State*> states; // This can change depending on how/when we want to allocate our resources, and if we need it to be like a stack

        // The main game objects, which exist for the entire game, independent of the current game state
        // All states will need access to these, but do not and should not have access to this state manager class
        // This is convienent to have in an object so we can pass a single reference to the states.
        GameObjects objects;
};

#endif
