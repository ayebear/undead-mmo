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
    setupWindow(windowTitle);
    setVSync();
    allocateStates();
}

StateManager::~StateManager()
{
    // Deallocate the game states
    for (auto& sPtr: statePtrs)
        delete sPtr;
    cout << "Successfully deallocated all states.\n";

    // Close the window
    objects.window.close();
}

void StateManager::setupWindow(string windowTitle)
{
    int windowWidth = objects.config.getOption("windowWidth").asInt();
    int windowHeight = objects.config.getOption("windowHeight").asInt();

    if (windowWidth > 0 && windowHeight > 0)
        createWindow(windowTitle, windowWidth, windowHeight);
    else
        createWindow(windowTitle);
}

void StateManager::createWindow(string windowTitle)
{
    // Create a window in fullscreen at the current resolution
    objects.vidMode = sf::VideoMode::getDesktopMode();

    // Only use a resolution 1080p or less - this is temporary until we can figure out how to handle multiple monitors
    if (objects.vidMode.width > 1920 || objects.vidMode.height > 1080)
    {
        auto videoModes = sf::VideoMode::getFullscreenModes();
        for (auto& vM: videoModes)
        {
            cout << vM.width << " x " << vM.height << "? ";
            if (vM.width <= 1920 && vM.height <= 1080)
            {
                objects.vidMode = vM;
                cout << "Using this!\n";
                break;
            }
            else
                cout << "Too big!\n";
        }
    }

    objects.window.create(objects.vidMode, windowTitle, sf::Style::Fullscreen);
}

void StateManager::createWindow(string windowTitle, int windowWidth, int windowHeight)
{
    // Create a normal window
    objects.vidMode = sf::VideoMode(windowWidth, windowHeight);
    objects.window.create(objects.vidMode, windowTitle, sf::Style::Close);
}

void StateManager::allocateStates()
{
    statePtrs[0] = new MainMenuState(objects);
    statePtrs[1] = new LoginState(objects);
    statePtrs[2] = new PlayGameState(objects);
    statePtrs[3] = new ErrorState(objects);
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
