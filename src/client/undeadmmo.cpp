// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "undeadmmo.h"
#include "mainmenustate.h"
#include "loginstate.h"
#include "gamestate.h"
#include "errorstate.h"
#include "messagestate.h"

UndeadMMO::UndeadMMO(const std::string& windowTitle)
{
    objects.setupWindow(windowTitle);
    allocateStates();
}

void UndeadMMO::start()
{
    states.startLoop("Menu");
}

void UndeadMMO::allocateStates()
{
    states.addState("Menu", new MainMenuState(objects));
    states.addState("Login", new LoginState(objects));
    states.addState("Game", new GameState(objects));
    states.addState("Error", new ErrorState(objects));
    states.addState("Message", new MessageState(objects));
}
