// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"
#include "state.h"
#include "stateaction.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
    deallocateStates();
}

void StateManager::addState(State* newState)
{
    statePtrs.emplace_back(newState);
}

void StateManager::startLoop(unsigned int firstState)
{
    StateAction action;
    action.pushState(firstState);
    while (action.isNotExit())
        handleAction(action);
}

void StateManager::deallocateStates()
{
    // Call the remaining onPop functions
    while (!stateStack.empty())
    {
        statePtrs[stateStack.top()]->onPop();
        stateStack.pop();
    }
    // Delete the allocated states
    for (auto& sPtr: statePtrs)
        sPtr.reset();
}

void StateManager::handleAction(StateAction& action)
{
    // Do something depending on the command
    switch (action.getCommand())
    {
        case StateCommand::Push:
            push(action.getType());
            break;
        case StateCommand::Pop:
            pop();
            break;
        default:
            break;
    }

    // Run the current state (on the top of the stack)
    if (!stateStack.empty())
        action = statePtrs[stateStack.top()]->start(action.getArgs());
    else // If the stack is empty
        action.exitGame(); // Exit the game
}

void StateManager::push(unsigned int type)
{
    if (type < statePtrs.size())
    {
        stateStack.push(type);
        statePtrs[type]->onPush();
    }
    // If this fails due to a bad type, the current state will just start again.
}

void StateManager::pop()
{
    if (!stateStack.empty())
    {
        statePtrs[stateStack.top()]->onPop();
        stateStack.pop();
    }
}
