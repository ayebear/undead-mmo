// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "stateaction.h"

bool StateAction::shouldContinue()
{
    return (command == StateCommand::Continue);
}

bool StateAction::isNotExit()
{
    return (command != StateCommand::Exit);
}

void StateAction::pushState(int stateType)
{
    command = StateCommand::Push;
    type = stateType;
}

void StateAction::pushState(int stateType, const StateArgs& theArgs)
{
    command = StateCommand::Push;
    type = stateType;
    args = theArgs;
}

void StateAction::popState()
{
    command = StateCommand::Pop;
}

void StateAction::exitGame()
{
    command = StateCommand::Exit;
}

void StateAction::reset()
{
    command = 0;
    type = 0;
    args.clear();
}

int StateAction::getCommand() const
{
    return command;
}

int StateAction::getType() const
{
    return type;
}

const StateArgs& StateAction::getArgs() const
{
    return args;
}
