#ifndef _BREAKOUTMESSAGES_H
#define _BREAKOUTMESSAGES_H

#include <SFML/Window.hpp>

#include <OCS/Messaging.hpp>
#include "OCS/Misc/config.hpp"

struct InputOccurred : public ocs::Message<InputOccurred>
{
    InputOccurred(const ocs::Transceiver& _transceiver, sf::Event& _event) : Message(_transceiver), event(_event) {}

    sf::Event event;
};

struct Window : public ocs::Message<Window>
{
    Window(const ocs::Transceiver& _transceiver, sf::RenderWindow& _window) : Message(_transceiver), window(_window) {}

    sf::RenderWindow& window;
};

struct CollisionOccurred : public ocs::Message<CollisionOccurred>
{
    CollisionOccurred(const ocs::Transceiver& _transceiver, ocs::ID _firstObject, ocs::ID _secondObject) :
                          Message(_transceiver), firstObject(_firstObject), secondObject(_secondObject) {}

    ocs::ID firstObject;
    ocs::ID secondObject;
};

#endif
