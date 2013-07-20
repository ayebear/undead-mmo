#include "gameobjects.h"
#include <iostream>
#include <string>

using namespace std;

const string GameObjects::configFilename = "options.txt";
const ConfigFile::ConfigMap GameObjects::defaultOptions = {
{"server", Option("ayebear.com")},
{"useVerticalSync", Option("true")}
};

GameObjects::~GameObjects()
{
    window.close();
}

void GameObjects::loadConfig()
{
    // TODO: Make sure to setup default values as well as ranges in memory
    config.setDefaultOptions(defaultOptions);
    config.loadConfigFile(configFilename);
}

void GameObjects::loadFonts()
{
    if (!font.loadFromFile("data/fonts/Ubuntu-R.ttf"))
        exit(7);
    if (!fontBold.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(7);
    if (!fontMono.loadFromFile("data/fonts/UbuntuMono-R.ttf"))
        exit(7);
    if (!fontMonoBold.loadFromFile("data/fonts/UbuntuMono-B.ttf"))
        exit(7);
}

void GameObjects::setupWindow(string windowTitle)
{
    int windowWidth = config.getOption("windowWidth").asInt();
    int windowHeight = config.getOption("windowHeight").asInt();

    if (windowWidth > 0 && windowHeight > 0)
        createWindow(windowTitle, windowWidth, windowHeight);
    else
        createWindow(windowTitle);

    window.setVerticalSyncEnabled(config.getOption("useVerticalSync").asBool()); // Set vsync from setting
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
