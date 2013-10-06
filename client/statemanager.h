// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <string>
#include <memory>
#include "gameobjects.h"
#include "state.h"

/*
This class handles the allocation/deallocation/starting/changing of all types of game states.
Also contains the instance of the GameObjects object, which is passed into the states when they are allocated.
*/
class StateManager
{
    public:
        StateManager(std::string); // Takes window title
        ~StateManager();

        void startLoop(const StateAction&); // The main loop that runs until a state returns an exit action

    private:
        void allocateStates();
        void deallocateStates();

        void handleAction(StateAction&);
        void push(unsigned int);
        void pop();

        std::vector<unsigned int> stateStack; // Represents a stack of the states
        std::unique_ptr<State> statePtrs[StateType::TotalTypes]; // Pointers to instances of all of the state types

        // The main game objects, which exist for the entire game, independent of the current game state
        // All states will need access to these, but do not and should not have access to this state manager class
        // This is convienent to have in an object so we can pass a single reference to the states.
        GameObjects objects;
};

#endif
