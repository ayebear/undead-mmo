// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"
#include "mainmenustate.h"
#include "loginstate.h"
#include "playgamestate.h"
#include "errorstate.h"
#include "messagestate.h"

StateManager::StateManager(std::string windowTitle)
{
    objects.setupWindow(windowTitle);
    allocateStates();
}

StateManager::~StateManager()
{
    // Deallocate the game states
    deallocateStates();
}

void StateManager::allocateStates()
{
    // TODO: In the future consider making this class fully generic and have an addState function,
    // which would add a state pointer. This could be identified by a string or custom enum.
    // It could use a map of keys and state pointers, with the key passed in as the template type.
    statePtrs[0].reset(new MainMenuState(objects));
    statePtrs[1].reset(new LoginState(objects));
    statePtrs[2].reset(new PlayGameState(objects));
    statePtrs[3].reset(new ErrorState(objects));
    statePtrs[4].reset(new MessageState(objects));
}

void StateManager::deallocateStates()
{
    // Call the remaining onPop functions
    while (!stateStack.empty())
    {
        statePtrs[stateStack.back()]->onPop();
        stateStack.pop_back();
    }
    // Delete the allocated states
    for (auto& sPtr: statePtrs)
        sPtr.reset();
    //std::cout << "Successfully deallocated all states.\n";
}

void StateManager::startLoop(unsigned int firstState)
{
    StateAction action;
    action.pushState(firstState);
    while (action.isNotExit())
        handleAction(action);
    //std::cout << "StateManager loop has ended.\n";
}

void StateManager::handleAction(StateAction& action)
{
    // Do something depending on the command
    switch (action.getCommand())
    {
        case StateCommand::Push:
            //std::cout << "Received push command for state type " << action.getType() << endl;
            push(action.getType());
            break;
        case StateCommand::Pop:
            //std::cout << "Received pop command.\n";
            pop();
            break;
        default:
            std::cerr << "Error: StateManager received an unknown command.\n";
            break;
    }

    // Run the current state (on the top of the stack)
    if (!stateStack.empty())
        action = statePtrs[stateStack.back()]->start(action.getArgs());
    else // If the stack is empty
        action.exitGame(); // Exit the game
}

void StateManager::push(unsigned int type)
{
    if (type < StateType::TotalTypes)
    {
        stateStack.push_back(type);
        statePtrs[type]->onPush();
    }
    // If this fails due to a bad type, the current state will just start again.
}

void StateManager::pop()
{
    if (!stateStack.empty())
    {
        statePtrs[stateStack.back()]->onPop();
        stateStack.pop_back();
    }
}
