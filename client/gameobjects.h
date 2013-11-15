// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include "clientnetwork.h"
#include "configfile.h"
#include "music.h"

/*
This class contains the main game objects, such as the window, networking, config file, and fonts.
This also handles creating the window and setting up the objects it contains so other classes don't have to.
*/
class GameObjects: sf::NonCopyable
{
    public:
        GameObjects();
        ~GameObjects();
        void setupWindow(const std::string&); // Reads window configuration and called createWindow

        sf::VideoMode vidMode; // Window size, color settings, etc.
        sf::Vector2u windowSize; // The current window size (if we decide to make the game resizable, this will need to be a function)
        sf::RenderWindow window; // The main window
        ClientNetwork netManager; // Handles communications with the server
        sf::Font font, fontBold, fontMono, fontMonoBold; // Some fonts
        ConfigFile config; // The main configuration file
        Music music;

    private:
        void createWindow(const std::string&, int, int, bool, bool); // Create a new window
        sf::VideoMode getMaxVidMode() const;

        void loadFonts(); // Loads font files

        static const ConfigFile::ConfigMap defaultOptions;
};

#endif
