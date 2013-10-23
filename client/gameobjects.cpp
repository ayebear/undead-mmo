// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "gameobjects.h"
#include <iostream>
#include <string>
#include "paths.h"

const ConfigFile::ConfigMap GameObjects::defaultOptions = {
    {"Window",{
        {"useVerticalSync", makeOption(true)},
        {"fullscreen", makeOption(true)},
        {"windowWidth", makeOption(0)},
        {"windowHeight", makeOption(0)}
        }
    },
    {"Credentials",{
        {"server", Option("127.0.0.1")},
        {"username", Option("test")},
        {"password", Option("password")}
        }
    },
    {"Hotkeys",{
        {"moveUp", Option("Up")},
        {"moveDown", Option("Down")},
        {"moveLeft", Option("Left")},
        {"moveRight", Option("Right")},
        {"moveUpAlt", Option("W")},
        {"moveDownAlt", Option("S")},
        {"moveLeftAlt", Option("A")},
        {"moveRightAlt", Option("D")},
        {"toggleInventory", Option("Space")},
        {"toggleInventoryAlt", Option("B")},
        {"showChat", Option("Return")},
        {"hideChat", Option("Escape")},
        {"takeScreenshot", Option("F1")},
        {"addSlot", Option("K")},
        {"removeSlot", Option("L")}
        }
    },
    {"Developer",{
        {"showFps", makeOption(false)},
        {"rewriteConfigFile", makeOption(false)}
        }
    }
};

GameObjects::GameObjects():
    config(Paths::clientConfigFile, defaultOptions) // Config file gets loaded here
{
    loadFonts();
}

GameObjects::~GameObjects()
{
    window.close();
    if (config.getOption("rewriteConfigFile", "Developer").asBool())
        config.writeToFile();
}

void GameObjects::setupWindow(const std::string& windowTitle)
{
    config.setSection("Window");
    int windowWidth = config["windowWidth"].asInt();
    int windowHeight = config["windowHeight"].asInt();
    bool fullscreen = config["fullscreen"].asBool();
    bool vsync = config["useVerticalSync"].asBool();
    config.setSection();

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
        vidMode = getMaxVidMode();

    // Either create the window in fullscreen or windowed mode
    window.create(vidMode, windowTitle, (fullscreen ? sf::Style::Fullscreen : sf::Style::Close));

    // Set the window's vertical sync setting
    window.setVerticalSyncEnabled(vsync);

    // Cache the window size so there is no need for expensive calls to window.getSize()
    windowSize.x = vidMode.width;
    windowSize.y = vidMode.height;
}

sf::VideoMode GameObjects::getMaxVidMode() const
{
    // Get the current screen resolution
    sf::VideoMode videoMode = sf::VideoMode::getDesktopMode();

    // Only use a resolution 1080p or less - this is temporary until we can figure out how to handle multiple monitors
    if (videoMode.width > 1920 || videoMode.height > 1080)
    {
        auto videoModes = sf::VideoMode::getFullscreenModes();
        for (auto& vM: videoModes)
        {
            std::cout << vM.width << " x " << vM.height << "? ";
            if (vM.width <= 1920 && vM.height <= 1080)
            {
                videoMode = vM;
                std::cout << "Using this!\n";
                break;
            }
            else
                std::cout << "Too big!\n";
        }
    }

    return videoMode;
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
