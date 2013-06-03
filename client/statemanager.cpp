// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "statemanager.h"
#include "mainmenustate.h"
#include "loginstate.h"
#include "playgamestate.h"
#include "errorstate.h"

StateManager::StateManager(std::string windowTitle, int windowWidth, int windowHeight)
{
    // Create a normal window
    objects.vidMode = sf::VideoMode(windowWidth, windowHeight);
    objects.window.create(objects.vidMode, windowTitle);

    // TODO: Have an option for this
    objects.window.setVerticalSyncEnabled(true);
}

StateManager::StateManager(std::string windowTitle)
{
    // Create a window in fullscreen at the current resolution
    objects.vidMode = sf::VideoMode::getDesktopMode();
    objects.window.create(objects.vidMode, windowTitle, sf::Style::Fullscreen);

    // TODO: Have an option for this
    objects.window.setVerticalSyncEnabled(true);
}

StateManager::~StateManager()
{
    // Deallocate the game states
    while (!states.empty())
    {
        delete states.back();
        states.pop_back();
    }

    cout << "Successfully deallocated all states.\n";

    // Close the window
    objects.window.close();
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
    if (states.empty())
        fail();
    else
        return states.back()->start(action.getArgs());

    return action;
}

void StateManager::push(int type)
{
    // Allocate a new state object based on the type
    State* statePtr = nullptr;
    switch (type)
    {
        case StateType::Menu:
            statePtr = new MainMenuState(objects);
            break;
        case StateType::Login:
            statePtr = new LoginState(objects);
            break;
        case StateType::Game:
            statePtr = new PlayGameState(objects);
            break;
        case StateType::Error:
            statePtr = new ErrorState(objects);
            break;
        default:
            break;
    }
    if (statePtr == nullptr)
        fail();

    // Save that pointer in the stack
    states.push_back(statePtr);

    // Another approach could be to use a stack of numbers, and a preallocated array with all the types of states.
    // This would be much faster and would never have to allocate more memory, or even deallocate memory.

    // TODO: Use smart pointers for better safety!
}

void StateManager::pop()
{
    if (!states.empty())
    {
        delete states.back();
        states.pop_back();
    }
}

void StateManager::fail()
{
    std::cout << "Something with the state manager went terribly wrong.\n";
    exit(123);
}
