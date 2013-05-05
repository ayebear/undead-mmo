// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include "resources.h"
#include "../shared/other.h"
#include "../shared/map.h"

Resources::Resources()
{
}

void Resources::LoadFiles()
{
    LoadFonts();
    LoadImages();
    LoadTiles();
    LoadSounds();
}

void Resources::LoadFonts()
{
    // Load font files
    if (!font.loadFromFile("data/fonts/Ubuntu-R.ttf"))
        exit(Errors::Font);
    if (!fontBold.loadFromFile("data/fonts/Ubuntu-B.ttf"))
        exit(Errors::Font);
    if (!fontMono.loadFromFile("data/fonts/UbuntuMono-R.ttf"))
        exit(Errors::Font);
    if (!fontMonoBold.loadFromFile("data/fonts/UbuntuMono-B.ttf"))
        exit(Errors::Font);
}

void Resources::LoadImages()
{
    // Load character sprites
    if (!playerTex.loadFromFile("data/images/characters/character.png"))
        exit(Errors::Graphics);
    playerTex.setSmooth(true);
    if (!zombieTex.loadFromFile("data/images/characters/zombie.png"))
        exit(Errors::Graphics);
    zombieTex.setSmooth(true);
    if (!menuBgTex.loadFromFile("data/images/ui/MenuBackground.png"))
        exit(Errors::Graphics);
    menuBgTex.setSmooth(true);
}

void Resources::LoadTiles()
{
    // 16 = 1024 / 128
    int tileCount = tilesImageSize / Map::tileWidth;
    textures.resize(tileCount * tileCount);
    for (int y = 0; y < tileCount; y++)
    {
        for (int x = 0; x < tileCount; x++)
        {
            if (textures[y * tileCount + x].loadFromFile("data/images/tiles/tiles.png",
                sf::IntRect(x * Map::tileWidth, y * Map::tileWidth,
                (x + 1) * Map::tileWidth, (y + 1) * Map::tileWidth)))
                    exit(Errors::Graphics);
            textures[y * tileCount + x].setSmooth(true);
        }
    }
}

void Resources::LoadSounds()
{

}
