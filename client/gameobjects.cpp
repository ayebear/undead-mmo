#include "gameobjects.h"

const string GameObjects::configFilename = "options.txt";

void GameObjects::loadConfig()
{
    // TODO: Make sure to setup default values as well as ranges in memory
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
