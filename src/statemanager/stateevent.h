// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATEEVENT_H
#define STATEEVENT_H

#include <vector>
#include <string>
#include "configoption.h"

// This is used to identify the state
typedef std::string StateId;

// Arguments to be passed into the next state
typedef std::vector<std::string> StateArgs;

/*
This class is used to create events which are processed by the state manager.
They can also contain optional arguments which are passed to the next state.
*/
class StateEvent
{
    public:

        // The state manager takes action based on this
        enum Command
        {
            Continue = 0, // Continues running the current state
            Exit, // Exits the whole program
            Pop, // Goes back to the previous state
            Push // Adds a new state onto the stack
        };

        StateEvent(): command(0) {}
        StateEvent(int c, const StateId& s): command(c), id(s) {}
        StateEvent(int c, const StateId& s, const StateArgs& a): command(c), id(s), args(a) {}

        bool shouldContinue() const;
        bool isNotExit() const;
        void pushState(const StateId&);
        void pushState(const StateId&, const StateArgs&);
        void popState();
        void exitGame();
        void reset();

        int getCommand() const;
        const StateId& getId() const;
        const StateArgs& getArgs() const;

    private:
        int command; // The ID of a command, see the Command enum above
        StateId id; // The state ID of the new state to push
        StateArgs args; // State arguments, which are passed to the next state
};

#endif
