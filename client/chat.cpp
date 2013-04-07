#include "chat.h"

/*
For the chat input code:
Have a chat state that can be on/off. This should be turned on after a "T" pressed event is received, and turned off when "Escape" is pressed.
Use sf::Event::TextEntered to get the text entered, so we don't have to test for a bunch of different types of keys pressed.
*/

Chat::Chat()
{
    shown = false;
}
