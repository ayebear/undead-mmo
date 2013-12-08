// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "undeadmmo.h"
#include "mainmenustate.h"
#include "loginstate.h"
#include "playgamestate.h"
#include "errorstate.h"
#include "messagestate.h"
#include "states.h"

UndeadMMO::UndeadMMO(const std::string& windowTitle)
{
    objects.setupWindow(windowTitle);
    allocateStates();
}

void UndeadMMO::start()
{
    states.startLoop(States::Menu);
}

void UndeadMMO::allocateStates()
{
    states.addState(States::Menu, new MainMenuState(objects));
    states.addState(States::Login, new LoginState(objects));
    states.addState(States::Game, new PlayGameState(objects));
    states.addState(States::Error, new ErrorState(objects));
    states.addState(States::Message, new MessageState(objects));
}
