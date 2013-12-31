// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"

StateManager::StateManager()
{
}

StateManager::~StateManager()
{
    deallocateStates();
}

void StateManager::addState(const StateId& newId, BaseState* newState)
{
    // Using reset() prevents memory leaks
    statePtrs[newId].reset(newState);
}

void StateManager::removeState(const StateId& id)
{
    statePtrs.erase(id);
}

void StateManager::startLoop(const StateId& firstState)
{
    StateEvent event;
    event.pushState(firstState);
    while (event.isNotExit())
        handleEvent(event);
}

void StateManager::deallocateStates()
{
    // Call the remaining onPop functions
    while (!stateStack.empty())
    {
        statePtrs[stateStack.top()]->onPop();
        stateStack.pop();
    }
    // Remove and delete the allocated states
    statePtrs.clear();
}

void StateManager::handleEvent(StateEvent& event)
{
    // Do something depending on the command
    switch (event.getCommand())
    {
        case StateEvent::Command::Push:
            push(event.getId());
            break;
        case StateEvent::Command::Pop:
            pop();
            break;
        default:
            break;
    }

    // Run the current state (on the top of the stack)
    if (!stateStack.empty())
        event = statePtrs[stateStack.top()]->start(event.getArgs());
    else // If the stack is empty
        event.exitGame(); // Exit the game
}

void StateManager::push(const StateId& id)
{
    auto found = statePtrs.find(id);
    if (found != statePtrs.end())
    {
        stateStack.push(id);
        found->second->onPush();
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
