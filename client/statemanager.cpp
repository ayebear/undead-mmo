// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"
#include "mainmenustate.h"
#include "loginstate.h"
#include "playgamestate.h"
#include "errorstate.h"

StateManager::StateManager(std::string windowTitle)
{
    objects.loadConfig();
    objects.loadFonts();
    objects.setupWindow(windowTitle);
    setVSync();
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
}

void StateManager::deallocateStates()
{
    for (auto& sPtr: statePtrs)
        delete sPtr;
    cout << "Successfully deallocated all states.\n";
}

void StateManager::startLoop(const StateAction& theAction)
{
    StateAction action = theAction;
    while (action.isNotExit())
        action = handleAction(action);
    std::cout << "StateManager loop has ended.\n";
}

const StateAction& StateManager::handleAction(const StateAction& action)
{
    // Do something depending on the command
    switch (action.getCommand())
    {
        case StateCommand::Push:
            std::cout << "Received push command for state type " << action.getType() << endl;
            push(action.getType());
            break;
        case StateCommand::Pop:
            std::cout << "Received pop command.\n";
            pop();
            break;
        default:
            std::cout << "Received unknown command.\n";
            break;
    }

    // Run the current state (on the top of the stack)
    if (!stateStack.empty())
        return statePtrs[stateStack.back()]->start(action.getArgs());

    exit(123); // The game should exit when it gets here, this means the stack is empty
    return action; // Only here to suppress the compiler warning...
}

void StateManager::push(int type)
{
    stateStack.push_back(type);
}

void StateManager::pop()
{
    if (!stateStack.empty())
        stateStack.pop_back();
}

void StateManager::setVSync()
{
    objects.window.setVerticalSyncEnabled(objects.config.getOption("useVerticalSync").asBool());
}
