// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "undeadmmo.h"
#include "mainmenustate.h"
#include "loginstate.h"
#include "playgamestate.h"
#include "errorstate.h"
#include "messagestate.h"

UndeadMMO::UndeadMMO(const std::string& windowTitle)
{
    objects.setupWindow(windowTitle);
    allocateStates();
}

void UndeadMMO::start()
{
    states.startLoop(StateType::Menu);
}

void UndeadMMO::allocateStates()
{
    states.addState(new MainMenuState(objects));
    states.addState(new LoginState(objects));
    states.addState(new PlayGameState(objects));
    states.addState(new ErrorState(objects));
    states.addState(new MessageState(objects));
}
