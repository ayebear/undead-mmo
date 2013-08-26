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
    cout << "Successfully deallocated all states.\n";
}

void StateManager::startLoop(const StateAction& theAction)
{
    StateAction action = theAction;
    while (action.isNotExit())
        handleAction(action);
    cout << "StateManager loop has ended.\n";
}

void StateManager::handleAction(StateAction& action)
{
    // Do something depending on the command
    switch (action.getCommand())
    {
        case StateCommand::Push:
            cout << "Received push command for state type " << action.getType() << endl;
            push(action.getType());
            break;
        case StateCommand::Pop:
            cout << "Received pop command.\n";
            pop();
            break;
        default:
            cout << "Received unknown command.\n";
            break;
    }

    // Run the current state (on the top of the stack)
    if (!stateStack.empty())
        action = statePtrs[stateStack.back()]->start(action.getArgs());
    else // If the stack is empty
        action.exitGame(); // Exit the game
}

void StateManager::push(int type)
{
    if (type >= 0 && type < StateType::TotalTypes)
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
