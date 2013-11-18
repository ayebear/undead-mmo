// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef UNDEADMMO_H
#define UNDEADMMO_H

#include <string>
#include "gameobjects.h"
#include "statemanager.h"

// This is the "main" class, so that we don't junk up main.cpp
class UndeadMMO
{
    public:
        UndeadMMO(const std::string&);
        void start();

    private:
        void allocateStates();

        // The state manager, which manages switching between the game states
        StateManager states;

        // The main game objects, which exist for the entire game, independent of the current game state.
        // All states need access to this, so a reference is passed to the states when they are allocated.
        GameObjects objects;
};

#endif
