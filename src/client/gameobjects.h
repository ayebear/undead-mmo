// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <SFML/Graphics.hpp>
#include "configfile.h"
#include "musicplayer.h"
#include "soundplayer.h"
#include "client.h"
#include "accountclient.h"
#include "packetbuilder.h"

/*
This class contains the main game objects, such as the window, networking, config file, and fonts.
This also handles creating the window and setting up the objects it contains so other classes don't have to.
All of the states are given access to this.
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
        net::Client client; // Used for communicating with a server
        AccountClient accountClient; // For handling logging in and creating accounts
        PacketBuilder packetBuilder; // Creates and sends game-specific packets
        sf::Font font, fontBold, fontMono, fontMonoBold; // Some fonts
        cfg::File config; // The main configuration file
        MusicPlayer music; // The music player
        SoundPlayer sound; // The sound effects player

    private:
        void createWindow(const std::string&, int, int, bool, bool); // Create a new window
        void loadFonts(); // Loads font files

        static const cfg::File::ConfigMap defaultOptions;
};

#endif
