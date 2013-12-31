// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "stateevent.h"

bool StateEvent::shouldContinue() const
{
    return (command == Command::Continue);
}

bool StateEvent::isNotExit() const
{
    return (command != Command::Exit);
}

void StateEvent::pushState(const StateId& stateId)
{
    command = Command::Push;
    id = stateId;
    args.clear();
}

void StateEvent::pushState(const StateId& stateId, const StateArgs& stateArgs)
{
    command = Command::Push;
    id = stateId;
    args = stateArgs;
}

void StateEvent::popState()
{
    command = Command::Pop;
}

void StateEvent::exitGame()
{
    command = Command::Exit;
}

void StateEvent::reset()
{
    command = 0;
    id.clear();
    args.clear();
}

int StateEvent::getCommand() const
{
    return command;
}

const StateId& StateEvent::getId() const
{
    return id;
}

const StateArgs& StateEvent::getArgs() const
{
    return args;
}
