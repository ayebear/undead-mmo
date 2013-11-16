// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef STATEACTION_H
#define STATEACTION_H

#include <vector>
#include <string>

typedef std::vector<std::string> StateArgs; // Later this could be a struct if we need more than just strings

namespace StateCommand
{
    enum Command
    {
        Continue = 0,
        Exit, // Exits the whole program
        Pop, // Goes back to the previous state
        Push // Adds a new state onto the stack
    };
}

// Check the TODO in the StateManager class about this
namespace StateType
{
    // These are just numeric IDs for different game state types
    enum Type
    {
        Menu = 0,
        Login,
        Game,
        Error,
        Message,
        TotalTypes
    };
}

/*
This class is used to create "actions" which are processed by the state manager.
They can also contain optional arguments which are passed to the next state.
*/
class StateAction
{
    public:
        StateAction(): command(0), type(0) {}
        StateAction(int c, unsigned int t): command(c), type(t) {}
        StateAction(int c, unsigned int t, const StateArgs& a): command(c), type(t), args(a) {}

        bool shouldContinue() const;
        bool isNotExit() const;
        void pushState(unsigned int);
        void pushState(unsigned int, const StateArgs&);
        void popState();
        void exitGame();
        void reset();

        int getCommand() const;
        unsigned int getType() const;
        const StateArgs& getArgs() const;
        // May also need setters, but the other functions should do the job in a more encapsulated way

    private:
        int command; // The ID of a command, see the Command enum in State class
        unsigned int type; // State type, see the Type enum in State class (only applies to a push command)
        StateArgs args; // State arguments, which are passed to the next state
};

#endif
