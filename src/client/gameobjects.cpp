// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "gameobjects.h"
#include <iostream>
#include <string>
#include "paths.h"

const cfg::File::ConfigMap GameObjects::defaultOptions = {
    {"Window",{
        {"useVerticalSync", cfg::makeOption(true)},
        {"fullscreen", cfg::makeOption(true)},
        {"windowWidth", cfg::makeOption(0)},
        {"windowHeight", cfg::makeOption(0)}
        }
    },
    {"Credentials",{
        {"server", cfg::Option("127.0.0.1")},
        {"username", cfg::Option("test")},
        {"password", cfg::Option("password")}
        }
    },
    {"Hotkeys",{
        {"moveUp", cfg::Option("Up")},
        {"moveDown", cfg::Option("Down")},
        {"moveLeft", cfg::Option("Left")},
        {"moveRight", cfg::Option("Right")},
        {"moveUpAlt", cfg::Option("W")},
        {"moveDownAlt", cfg::Option("S")},
        {"moveLeftAlt", cfg::Option("A")},
        {"moveRightAlt", cfg::Option("D")},
        {"toggleInventory", cfg::Option("Space")},
        {"toggleInventoryAlt", cfg::Option("B")},
        {"showChat", cfg::Option("Return")},
        {"hideChat", cfg::Option("Escape")},
        {"takeScreenshot", cfg::Option("F1")},
        {"addSlot", cfg::Option("K")},
        {"removeSlot", cfg::Option("L")}
        }
    },
    {"Developer",{
        {"showFps", cfg::makeOption(false)},
        {"rewriteConfigFile", cfg::makeOption(false)}
        }
    }
};

GameObjects::GameObjects():
    config(Paths::clientConfigFile, defaultOptions, true), // Config file gets loaded here
    music(Paths::musicConfigFile),
    sound(Paths::soundsConfigFile)
{
    loadFonts();
}

GameObjects::~GameObjects()
{
    window.close();
    if (config("rewriteConfigFile", "Developer").toBool())
        config.writeToFile();
}

void GameObjects::setupWindow(const std::string& windowTitle)
{
    config.useSection("Window");
    int windowWidth = config("windowWidth").toInt();
    int windowHeight = config("windowHeight").toInt();
    bool fullscreen = config("fullscreen").toBool();
    bool vsync = config("useVerticalSync").toBool();
    config.useSection();

    createWindow(windowTitle, windowWidth, windowHeight, fullscreen, vsync);
}

void GameObjects::createWindow(const std::string& windowTitle, int windowWidth, int windowHeight, bool fullscreen, bool vsync)
{
    bool windowSizeValid = (windowWidth >= 640 && windowHeight >= 480);

    // If windowed mode was specified and the sizes were invalid, use the default size
    if (!fullscreen && !windowSizeValid)
    {
        windowWidth = 1024;
        windowHeight = 768;
        windowSizeValid = true;
    }

    // Use the specified window width and height if they are valid
    if (windowSizeValid)
        vidMode = sf::VideoMode(windowWidth, windowHeight);
    else
        vidMode = sf::VideoMode::getDesktopMode();

    // Either create the window in fullscreen or windowed mode
    window.create(vidMode, windowTitle, (fullscreen ? sf::Style::Fullscreen : sf::Style::Close));

    // Set the window's vertical sync setting
    window.setVerticalSyncEnabled(vsync);

    // Cache the window size so there is no need for expensive calls to window.getSize()
    windowSize.x = vidMode.width;
    windowSize.y = vidMode.height;
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
