// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include "clientnetwork.h"
#include "configfile.h"

/*
This class contains the main game objects, such as the window, networking, config file, and fonts.
This also handles creating the window and setting up the objects it contains so other classes don't have to.
*/
class GameObjects: sf::NonCopyable
{
    public:
        GameObjects();
        ~GameObjects();
        void loadConfig(); // Loads the config file
        void loadFonts(); // Loads font files
        void setupWindow(string); // Calls createWindow based on configuration
        void createWindow(string); // Fullscreen
        void createWindow(string, int, int); // Windowed

        sf::VideoMode vidMode;
        sf::RenderWindow window;
        ClientNetwork netManager;
        sf::Font font, fontBold, fontMono, fontMonoBold;
        ConfigFile config;

    private:
        static const string configFilename;
        static const ConfigFile::Section defaultOptions;
};

#endif
