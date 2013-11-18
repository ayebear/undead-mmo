// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <vector>
#include <stack>
#include <string>
#include <memory>

class State;
class StateAction;

/*
This class handles the deallocation/starting/changing of all types of game states.
This class is also generic, but depends on having the state base class.
*/
class StateManager
{
    public:
        StateManager();
        ~StateManager();

        void addState(State*); // Adds a state pointer to the vector
        void startLoop(unsigned int); // The main loop that runs until a state returns an exit action

    private:
        void deallocateStates();

        void handleAction(StateAction&);
        void push(unsigned int);
        void pop();

        std::stack<unsigned int> stateStack; // Represents a stack of the states
        typedef std::unique_ptr<State> StatePtr; // Unique pointer to a state
        std::vector<StatePtr> statePtrs; // Pointers to instances of all of the state types
};

#endif
