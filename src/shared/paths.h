#ifndef PATHS_H
#define PATHS_H

#include <string>

namespace Paths
{
    // Directories
    const std::string dataDir = "data/";
    const std::string imagesDir = dataDir + "images/";
    const std::string fontsDir = dataDir + "fonts/";
    const std::string screenshotsDir = dataDir + "screenshots/";

    // Filenames

    // Images
    const std::string entitiesImage = imagesDir + "entities.png";
    const std::string itemIcons = imagesDir + "itemicons.png";
    const std::string menuBgImage = imagesDir + "menubg.png";
    const std::string tilesImage = imagesDir + "tiles.png";

    // Fonts
    const std::string regularFont = fontsDir + "Ubuntu-R.ttf";
    const std::string boldFont = fontsDir + "Ubuntu-B.ttf";
    const std::string monoFont = fontsDir + "UbuntuMono-R.ttf";
    const std::string monoBoldFont = fontsDir + "UbuntuMono-B.ttf";

    // Other
    const std::string clientConfigFile = "game.cfg";
    const std::string serverConfigFile = "server.cfg";
    const std::string masterServersConfig = "masterservers.txt";
}

#endif
