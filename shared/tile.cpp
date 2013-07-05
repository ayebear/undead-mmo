// See the file COPYRIGHT.txt for authors and copyright information.
// See the file LICENSE.txt for copying conditions.

#include <iostream>
#include "tile.h"

std::vector<sf::Texture> Tile::textures;

Tile::Tile()
{
    setID(0);
    setPos(0, 0);
}

Tile::Tile(TileID tileID, int x, int y)
{
    setID(tileID);
    setPos(x, y);
}

void Tile::loadTileTextures()
{
    if (!textures.empty())
    {
        std::cout << "Tile textures already loaded.\n";
        return;
    }

    std::cout << "Loading tile textures...\n";
    // Load the entire image into memory
    sf::Image* tilesImage = new sf::Image;
    tilesImage->loadFromFile("data/images/tiles/tiles.png");
    // Split up the image into separate textures on your GPU
    int tileCount = 1024 / tileWidth; // 16 = 1024 / 128
    textures.resize(tileCount * tileCount);
    for (int y = 0; y < tileCount; y++)
    {
        for (int x = 0; x < tileCount; x++)
        {
            if (!textures[y * tileCount + x].loadFromImage(*tilesImage,
                sf::IntRect(x * tileWidth, y * tileWidth,
                (x + 1) * tileWidth, (y + 1) * tileWidth)))
                    exit(1);
            textures[y * tileCount + x].setSmooth(false);
        }
    }
    // Delete the image from memory
    delete tilesImage;
    std::cout << "Done loading tile textures!\n";
}

void Tile::setID(TileID tileID)
{
    ID = tileID;

    if (ID < 128)
        walkable = true;
    else
        walkable = false;

    if (ID < textures.size())
        sprite.setTexture(textures[ID]);
    //else
        //std::cout << "Fatal Error: Failed to set texture. textures.size() = " << textures.size() << "\n";
}

const TileID Tile::getID() const
{
    return ID;
}

void Tile::setPos(int x, int y)
{
    sprite.setPosition(x, y);
}

bool Tile::isWalkable()
{
    return walkable;
}

void Tile::draw(sf::RenderTarget& window, sf::RenderStates states) const
{
    window.draw(sprite);
}
