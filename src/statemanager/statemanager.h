// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <string>
#include <stack>
#include <map>
#include <memory>
#include "basestate.h"
#include "stateevent.h"

/*
This class handles the deallocation/starting/changing of BaseState sub-classes.
This class is also generic, but depends on having the BaseState class and StateEvent class.
*/
class StateManager
{
    public:
        StateManager();
        ~StateManager();

        void addState(const StateId&, BaseState*); // Adds a state pointer to the map (Note that this takes ownership of the state object)
        void removeState(const StateId&); // Removes and deallocates a state from the map
        void startLoop(const StateId&); // The main loop that runs until a state returns an exit event

    private:
        void deallocateStates();

        void handleEvent(StateEvent&); // Handles a StateEvent object
        void push(const StateId&); // Adds a state onto the stack
        void pop(); // Removes the last pushed state from the stack

        std::stack<StateId> stateStack; // Represents a stack of the states
        typedef std::unique_ptr<BaseState> StatePtr; // Unique pointer to a state
        std::map<StateId, StatePtr> statePtrs; // Pointers to instances of the state types, accessed by the StateId
};

#endif
