#ifndef GAMEHOTKEYS_H
#define GAMEHOTKEYS_H

#include <vector>
#include <string>
#include <map>
#include <SFML/Window/Keyboard.hpp>
//#include "configfile.h"

typedef sf::Keyboard::Key Key;

/*
This class is used for the customization of hotkeys
Eventually this could be a fully generic event handler class
*/
class GameHotkeys
{
    public:
        void addKey(const std::string&);

        // Just use a vector for now to keep the same code structure
        const Key operator[](int) const; // Returns the user-defined key code for an event

        // Maybe use a map of key codes and function pointers
        //void handleEvent(int event) const;
        // This would be 100% generic, you would add your own function pointers and event codes
        // It would need to properly handle key pressed and key released events, as well as real-time inputs
        // At this point it should be called EventHandler or something

    private:
        Key strToKey(const std::string&);

        static const std::map<std::string,Key> keyNames;

        std::vector<Key> keys;

};

#endif
