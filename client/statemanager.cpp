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
    objects.loadConfig();
    objects.loadFonts();
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
    statePtrs[0] = new MainMenuState(objects);
    statePtrs[1] = new LoginState(objects);
    statePtrs[2] = new PlayGameState(objects);
    statePtrs[3] = new ErrorState(objects);
    statePtrs[4] = new MessageState(objects);
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
        delete sPtr;
    cout << "Successfully deallocated all states.\n";
}

void StateManager::startLoop(const StateAction& theAction)
{
    StateAction action = theAction;
    while (action.isNotExit())
        action = handleAction(action);
    cout << "StateManager loop has ended.\n";
}

const StateAction& StateManager::handleAction(const StateAction& action)
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
        return statePtrs[stateStack.back()]->start(action.getArgs());

    exit(901); // The game should exit when it gets here, this means the stack is empty
    return action; // Only here to suppress the compiler warning...
}

void StateManager::push(int type)
{
    stateStack.push_back(type);
    statePtrs[type]->onPush();
}

void StateManager::pop()
{
    if (!stateStack.empty())
    {
        statePtrs[stateStack.back()]->onPop();
        stateStack.pop_back();
    }
}
