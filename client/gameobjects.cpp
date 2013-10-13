// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "gameobjects.h"
#include <iostream>
#include <string>
#include "paths.h"

using namespace std;

const ConfigFile::Section GameObjects::defaultOptions = {
    {"server", Option("127.0.0.1")},
    {"username", Option("test")},
    {"password", Option("password")},
    {"windowWidth", makeOption(1024, 640)},
    {"windowHeight", makeOption(768, 480)},
    {"useVerticalSync", makeOption(true)}
};

GameObjects::GameObjects()
{
    loadConfig();
    loadFonts();
}

GameObjects::~GameObjects()
{
    window.close();
}

void GameObjects::setupWindow(string windowTitle)
{
    int windowWidth = config["windowWidth"].asInt();
    int windowHeight = config["windowHeight"].asInt();

    if (windowWidth > 0 && windowHeight > 0)
        createWindow(windowTitle, windowWidth, windowHeight);
    else
        createWindow(windowTitle);

    window.setVerticalSyncEnabled(config["useVerticalSync"].asBool()); // Set vsync from setting

    windowSize.x = vidMode.width;
    windowSize.y = vidMode.height;
}

void GameObjects::createWindow(string windowTitle)
{
    // Create a window in fullscreen at the current resolution
    vidMode = sf::VideoMode::getDesktopMode();

    // Only use a resolution 1080p or less - this is temporary until we can figure out how to handle multiple monitors
    if (vidMode.width > 1920 || vidMode.height > 1080)
    {
        auto videoModes = sf::VideoMode::getFullscreenModes();
        for (auto& vM: videoModes)
        {
            cout << vM.width << " x " << vM.height << "? ";
            if (vM.width <= 1920 && vM.height <= 1080)
            {
                vidMode = vM;
                cout << "Using this!\n";
                break;
            }
            else
                cout << "Too big!\n";
        }
    }

    window.create(vidMode, windowTitle, sf::Style::Fullscreen);
}

void GameObjects::createWindow(string windowTitle, int windowWidth, int windowHeight)
{
    // Create a normal window
    vidMode = sf::VideoMode(windowWidth, windowHeight);
    window.create(vidMode, windowTitle, sf::Style::Close);
}

void GameObjects::loadConfig()
{
    config.setDefaultOptions(defaultOptions);
    config.loadFromFile(Paths::clientConfigFile);
}

void GameObjects::loadFonts()
{
    if (!font.loadFromFile(Paths::regularFont))
        exit(301);
    if (!fontBold.loadFromFile(Paths::boldFont))
        exit(302);
    if (!fontMono.loadFromFile(Paths::monoFont))
        exit(303);
    if (!fontMonoBold.loadFromFile(Paths::monoBoldFont))
        exit(304);
}
