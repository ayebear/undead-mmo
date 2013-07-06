// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include "clientnetwork.h"
#include "configfile.h"

class GameObjects: sf::NonCopyable
{
    public:
        void loadConfig();
        void loadFonts();
        sf::VideoMode vidMode;
        sf::RenderWindow window;
        ClientNetwork netManager;
        sf::Font font, fontBold, fontMono, fontMonoBold;
        ConfigFile config;
    private:
        static const string configFilename;
};

#endif
